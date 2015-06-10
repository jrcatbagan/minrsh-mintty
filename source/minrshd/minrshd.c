/*
 * File: minrshd.c
 * Created: 2014, November 09
 *
 *
 * BSD License
 *
 * Copyright (c) 2015, Jarielle Catbagan <jrcatbagan@ca.rr.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <netinet/in.h>
#include <string.h>

#include <common/network.h>
#include <common/options.h>
#include <common/defines.h>
#include <common/util.h>
#include <core/comminit.h>
#include <crypt/aes.h>
#include <crypt/key.h>

#include <common/command.h>

int main(int argc, char **argv)
{
	struct net_info_t server, client;
	ssize_t bytes_read, bytes_written;
	bool done_state = false;
	char message[16];


	if (extract_options(&server, argc, argv) == -1) {
		fprintf(stderr, "error: options extraction failed\n");
		exit(EXIT_FAILURE);
	}


	/* set up the server */
        if (initialize_server(&server) == -1) {
                fprintf(stderr, "error: server initiation failed\n");
                exit(1);
        }


	accept_client_connection(&client, &server);
			  
	if (verify_client_initiation(client.fd) == -1) {
		fprintf(stderr, "error: invalid client initiation\n");
		close(client.fd);
		close(server.fd);
		exit(EXIT_FAILURE);
	}

	debug("valid client initiation\n");
	
	
	while (!done_state) {
		bzero(message, sizeof(message));
		bytes_read = read(client.fd, message, 16);
		debug("%d bytes read via read()\n", bytes_read);
		unsigned char controlcommand;

		int i;
		for (i = 0; i < 16; i++)
			debug("%0x ", message[i]);
		debug("\n");

		debug("invoking aes_decrypt()\n");
		//aes_decrypt(message, key);
		debug("aes_decrypt() finished\n");

		debug("command received: %s\n", message);
		if (!strcmp(message, "exit")) {
			done_state = true;
		}
		else {
			if (strstr(message, "cd")) {
				size_t str_length = strlen(message);
				if (str_length == 2 || str_length == 3) {
					/* need to fix this to actually change to the user's
					 * home directory that is using an instance of minrshd
					 */
					chdir("/home/jrcatbagan");
					debug("changing to hard-coded directory /home/jrcatbagan\n");
				}
				else {
					size_t arg_length = str_length - 3;
					char *argument = malloc(arg_length * sizeof(*argument));
					strcpy(argument, message + 3);
					debug("cd entered\n");
					debug("argument: %s - message: %s - argument length: %d\n",
							argument, message, arg_length);
					chdir(argument);
					free(argument);
				}
				send_controlcommand(client.fd, CMD_NO_DATA);
				controlcommand = receive_controlcommand(client.fd);
				if (controlcommand != ACK) {
					fprintf(stderr, "error: invalid acknowledgement\n");
					close(server.fd);
					close(client.fd);
					exit(1);
				}
			}
			else {
				FILE *command_pipe = popen(message, "r");
				
				char *command_output_buffer = malloc(sizeof(*command_output_buffer) * 16);;
				size_t command_output_buffer_length = 16;

				debug("\n");

				while (getline(&command_output_buffer, &command_output_buffer_length, 
							command_pipe) != -1) {
					debug("we have data to send\n");
					send_controlcommand(client.fd, CMD_MORE_DATA);
					debug("we are waiting to see if client ready to receive this data\n");
					controlcommand = receive_controlcommand(client.fd);
					if (controlcommand != ACK) {
						fprintf(stderr, "error: invalid acknowledgement\n");
						close(server.fd);
						close(client.fd);
						exit(1);
					}

					debug("we just received acknowledgement that the client is ready to receive the data\n");
					debug("the first batch of data to send is: \n");
					debug("\t%s - size %d\n", command_output_buffer, command_output_buffer_length);

					debug("we are sending the number of bytes in this batch of output\n");
					bytes_written = write(client.fd, &command_output_buffer_length,
								sizeof(command_output_buffer_length));

					unsigned int ndatablocks = (command_output_buffer_length + 16) / 16;
					unsigned int datablockindex;
					for (datablockindex = 0; datablockindex < ndatablocks; datablockindex++) {
						debug("\nsending datablock # %d\n", datablockindex + 1);
						bzero(message, sizeof(message));
						unsigned int base = 16 * datablockindex;
						int i, j;
						for (i = base, j = 0; i < (base + 16); i++, j++)
							message[j] = command_output_buffer[i];
						debug("writing data\n");
						bytes_written = write(client.fd, message, sizeof(message));
						
						debug("we are waiting for the client to acknowledge the datablock we sent\n");
						controlcommand = receive_controlcommand(client.fd);
						if (controlcommand != ACK) {
							fprintf(stderr, "error: invalid acknowledgement\n");
							close(server.fd);
							close(client.fd);
							exit(1);
						}

						debug("the data has been successfully sent\n");

						if ((datablockindex + 1) < ndatablocks) {
							debug("we have more data to send so let's notify the client\n");
							send_controlcommand(client.fd, CMD_MORE_DATA);
							controlcommand = receive_controlcommand(client.fd);
							if (controlcommand != ACK) {
								fprintf(stderr, "error: invalid acknowledgement\n");
								close(server.fd);
								close(client.fd);
								exit(1);
							}

							debug("the client acknowledges that they will receive more datablocks\n");

						}
						else {
							debug("we have NO more data to send so let's notify the client\n");
							send_controlcommand(client.fd, CMD_NO_DATA);
							controlcommand = receive_controlcommand(client.fd);
							if (controlcommand != ACK) {
								fprintf(stderr, "error: invalid acknowledgement\n");
								close(server.fd);
								close(client.fd);
								exit(1);
							}
							
							debug("the client acknowledges that they will not receive data anymore\n");
						}


					}


				}

				debug("we are notifying the client that we are done sending all data\n");
				send_controlcommand(client.fd, CMD_NO_DATA);
				debug("we sent that we have no more data\n");
				controlcommand = receive_controlcommand(client.fd);
				if (controlcommand != ACK) {
					fprintf(stderr, "error: invalid acknowledgement\n");
					close(server.fd);
					close(client.fd);
					exit(1);
				}

				debug("the client has notified us that they acknowledge that there is no more data to send\n");
				debug("\n");



				free(command_output_buffer);
				pclose(command_pipe);
			}
		}
	}

	close(server.fd);
	close(client.fd);
        exit(0);
}
