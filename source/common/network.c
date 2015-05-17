/*
 * File: network.c
 * Created: 2014, November 07
 *
 *
 * BSD License
 *
 * Copyright (C) 2015, Jarielle Catbagan <jrcatbagan@ca.rr.com>
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "../../include/common/network.h"


int initserver(int *serverfd, const char *ipaddr, uint16_t port)
{
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	int retval;

        if(fd < 0) {
                serverfd = NULL;
                return -1;
        }
	else {
	        *serverfd = fd;
	}
        struct sockaddr_in servername;

        servername.sin_family = AF_INET;
	retval = inet_aton(ipaddr, &(servername.sin_addr));
        if(retval == 0) {
                fprintf(stderr, "error: ip address is invalid\n");
                close(fd);
                serverfd = NULL;
                return -1;
        }
        servername.sin_port = htons(port);

        retval = bind(*serverfd, (struct sockaddr *) &servername,
                          sizeof(servername));
        if(retval != 0) {
                fprintf(stderr, "error: binding to arbitrary address failed\n");
                close(fd);
                serverfd = NULL;
                return -1;
        }
        
        retval = listen(*serverfd, 1);
        if(retval != 0) {
                fprintf(stderr, "error: listening on port %d failed\n", port);
                close(fd);
                serverfd = NULL;
                return -1;
        }
        else {
                return 0;
        }
}


int initclient(int *serverfd, const char *ipaddr, uint16_t port)
{
        int fd = socket(PF_INET, SOCK_STREAM, 0);
        if(fd < 0) {
                serverfd = NULL;
                return -1;
        }
        else {
                *serverfd = fd;
        }

        struct sockaddr_in servername;

        servername.sin_family = AF_INET;
        servername.sin_port = htons(port);
        int retval = inet_aton(ipaddr, &(servername.sin_addr));
        if(retval == 0) {
                fprintf(stderr, "error: ip address is invalid\n");
                close(fd);
                serverfd = NULL;
                return -1;
        }

        retval = connect(*serverfd, (struct sockaddr *) &servername,
                             sizeof(servername));
        if(retval != 0) {
		perror("error: ");
                fprintf(stderr, "error: connection to %s failed\n", ipaddr);
                close(fd);
                serverfd = NULL;
                return -1;
        }
        else {
                fprintf(stdout, "now connected to %s\n", ipaddr);
                return 0;
        }
}
