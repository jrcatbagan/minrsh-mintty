/*
 * File: network.h
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


#ifndef NETWORK_H
#define NETWORK_H

#include <stdint.h>

/* 'struct net_info_t' contains members that are initialized by 'extract_options()' in
 * /include/common/options.h
 *
 * the member within this structure must be dynamically allocated; since the intended
 * values stored into these variables are allocated by the system at program execution
 * start, no need to worry */
struct net_info_t
{
	char *ip_address;
	uint16_t port;
};

/* 
 * Initializes a server instance by opening a socket with the specified port 
 * number and binding any incoming address to the socket. If no errors occur 
 * the file descriptor of the socket is stored in the first parameter supplied
 * and a value status of 0 is returned. Otherwise the file descriptor is 
 * set to NULL and a value status of -1 is returned.
 *
 *
 * @param serverfd: Set to file descriptor returned from socket().
 *
 * @param port: The port number the server is listening on.
 *
 *
 * @retval 0: Server initialization was successful.
 * @retval 1: Server initialization was unsuccessful.
 */
int initserver(int *serverfd, const char *ipaddr, uint16_t port);


/*
 * Initializes a connection to the server identified by the
 * IP address and port number supplied. A value status of 0 is
 * returned if the connection was successful. Otherwise a value
 * status of -1 is returned to indicate that an error has occured.
 *
 *
 * @param serverfd: Set to the file descriptor returned from socket().
 *
 * @param ipaddr: The IP address represented as a character string
 * with the dot-notation.
 *
 * @param port: The port number the server is listening on.
 *
 *
 * @retval 0: Connection to server was successful.
 * @retval 1: Connection to server was not successful.
 */
int initclient(int *serverfd, const char *ipaddr, uint16_t port);

#endif /* NETWORK_H */
