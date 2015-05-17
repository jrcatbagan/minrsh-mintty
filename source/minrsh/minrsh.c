/*
 * File: minrsh.c
 * Created: 2014, November 09
 *
 *
 * BSD License
 *
 * Copyright (c) 2014, Jarielle Catbagan <jrcatbagan@ca.rr.com>
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
#include <crypt/aes.h>
#include <crypt/key.h>

int main(int argc, char **argv) 
{
        int serverfd;

	struct net_info_t net_info;

	enum flag_t err_flag = extract_options(&net_info, argc, argv);
	if (err_flag == SET) {
		fprintf(stderr, "error: options extraction failed\n");
		exit(EXIT_FAILURE);
	}

	/* set up the client */
        int retval = initclient(&serverfd, net_info.ip_address, net_info.port);
        if(retval == -1) {
                fprintf(stderr, "error: server connection failed\n");
                exit(1);
        }

	/* communication initiation sequence */
        unsigned char buffer[] = {0x55, 0xAA, 0x55, 0xAA};

        ssize_t bytes_written = write(serverfd, buffer, sizeof(buffer));
        ssize_t bytes_read;
	char *inbuffer = NULL;
	size_t n = 0;

	bool done_state = false;

	while (!done_state) {
		bytes_read = getline(&inbuffer, &n, stdin);
		
		n = bytes_read - 1;
		inbuffer[n] = '\0';
	
		char message[16];
		bzero(message, sizeof(message));
		strcpy(message, inbuffer);

		debug("invoking aes_encrypt()\n");
		aes_encrypt(message, key);
		debug("aes_encrypt() finished\n");

		debug("writing data via write()\n");
		bytes_written = write(serverfd, message, 16);
		debug("data written via write()\n");

		if(!strcmp(inbuffer, "exit"))
			done_state = true;
	}


	close(serverfd);

        exit(0);
}
