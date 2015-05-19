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

int main(int argc, char **argv)
{
        int serverfd;
	struct net_info_t net_info;
	ssize_t bytes_read, bytes_written;
	bool done_state = false;
	char message[16];

	if (extract_options(&net_info, argc, argv) == -1) {
		fprintf(stderr, "error: options extraction failed\n");
		exit(EXIT_FAILURE);
	}

	/* set up the server */
        int retval = initserver(&serverfd, net_info.ip_address, net_info.port);
        if(retval == -1) {
                fprintf(stderr, "error: server initiation failed\n");
                exit(1);
        }

        int clientfd;
        struct sockaddr_in clientname;
	int size = sizeof(clientname);
        clientfd = accept(serverfd, (struct sockaddr *) &clientname, &size);
			  
	if (verify_client_initiation(clientfd) == -1) {
		fprintf(stderr, "error: invalid client initiation\n");
		close(clientfd);
		close(serverfd);
		exit(EXIT_FAILURE);
	}
	else {
		fprintf(stdout, "valid client initiation\n");
	}

	while (!done_state) {
		bzero(message, sizeof(message));
		bytes_read = read(clientfd, message, 16);
		debug("%d bytes read via read()\n", bytes_read);

		debug("invoking aes_decrypt()\n");
		aes_decrypt(message, key);
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
			}
			else {
				FILE *command_pipe = popen(message, "r");
				char *command_output_buffer;
				size_t command_output_buffer_length = 0;

				printf("\n");
				while (getline(&command_output_buffer, &command_output_buffer_length, 
							command_pipe) != -1) {
					printf("\t%s", command_output_buffer);
				}

				free(command_output_buffer);
				pclose(command_pipe);
			}
		}
	}

	close(serverfd);
	close(clientfd);
        exit(0);
}
