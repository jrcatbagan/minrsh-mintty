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
			  
        unsigned char buffer[100];
        ssize_t bytes_read = read(clientfd, buffer, sizeof(buffer));
        if(bytes_read == 0 || bytes_read == -1) {
                fprintf(stdout, "no client data\n");
                exit(1);
        }
	
	enum flag_t cominvalid_flag = NOT_SET;

	int i;
	for (i = 0; i < bytes_read; i++) {
		printf("%x\n", buffer[i]);
        	if (i % 2) {
			if (!(buffer[i] == 0xAA))
				cominvalid_flag = SET;
		}
		else {
			if (!(buffer[i] == 0x55))
				cominvalid_flag = SET;
		}
	}

	if (cominvalid_flag == SET)
		printf("client connected; invalid communication initiation sequence\n");
	else
		printf("client connected; valid communication initiation sequence\n");

	bool done_state = false;
	while (!done_state) {
		bzero(buffer, sizeof(buffer));
		
		bytes_read = read(clientfd, buffer, 16);
		debug("%d bytes read via read()\n", bytes_read);

		char message[16];
		strncpy(message, buffer, bytes_read);

		debug("invoking aes_decrypt()\n");
		aes_decrypt(message, key);
		debug("aes_decrypt() finished\n");

		debug("data received: %s\n", message);

		if (!strcmp(message, "exit"))
			done_state = true;
	}

	close(serverfd);
	close(clientfd);
        exit(0);
}
