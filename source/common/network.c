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


int initialize_server(struct net_info_t *server)
{
	server->fd = socket(PF_INET, SOCK_STREAM, 0);
	int retval;

        if(server->fd < 0) {
                return -1;
        }

        server->name.sin_family = AF_INET;
	retval = inet_aton(server->ip_address, &(server->name.sin_addr));
        if(retval == 0) {
                fprintf(stderr, "error: ip address is invalid\n");
                close(server->fd);
                return -1;
        }
        server->name.sin_port = htons(server->port);

        retval = bind(server->fd, (struct sockaddr *) &server->name,
                          sizeof(server->name));
        if(retval != 0) {
                fprintf(stderr, "error: binding to arbitrary address failed\n");
                close(server->fd);
                return -1;
        }
        
        retval = listen(server->fd, 1);
        if(retval != 0) {
                fprintf(stderr, "error: listening on port %d failed\n", server->port);
                close(server->fd);
                return -1;
        }
        else {
                return 0;
        }
}


int connect_to_server(struct net_info_t *server)
{
	server->fd = socket(PF_INET, SOCK_STREAM, 0);
        if(server->fd < 0) {
                return -1;
        }

        server->name.sin_family = AF_INET;
        server->name.sin_port = htons(server->port);
        int retval = inet_aton(server->ip_address, &(server->name.sin_addr));
        if(retval == 0) {
                fprintf(stderr, "error: ip address is invalid\n");
                close(server->fd);
                return -1;
        }

        retval = connect(server->fd, (struct sockaddr *) &server->name,
                             sizeof(server->name));
        if(retval != 0) {
		perror("error: ");
                fprintf(stderr, "error: connection to %s failed\n", server->ip_address);
                close(server->fd);
                return -1;
        }
        else {
                fprintf(stdout, "now connected to %s\n", server->ip_address);
                return 0;
        }
}


void accept_client_connection(struct net_info_t *client, struct net_info_t *server)
{
	client->fd = accept(server->fd, (struct sockaddr *) &client->name, &client->name_size);
}
