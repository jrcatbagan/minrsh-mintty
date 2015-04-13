/*
 * File: client.c
 * Created: 09, November 2014
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
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <netinet/in.h>

#include "../utils/network.h"

int main(int argc, char **argv) 
{
        int serverfd;
	uint16_t port;
	const char *ip_address;

	char retoption;
	char *port_literal, *ipaddr_literal;
	struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"ip-address", required_argument, NULL, 'i'},
		{"port", required_argument, NULL, 'p'},
		{0, 0, 0, 0},
	};

	/* disable getopt_long from printing error messages */
	opterr = 0;

	enum flag_t {NOT_SET = 0, SET} h_flag, i_flag, p_flag, err_flag;
	h_flag = i_flag = p_flag = err_flag = NOT_SET;
	while((retoption = getopt_long(argc, argv, "hi:p:", 
				       long_options, NULL)) != -1) {
		switch(retoption) {
		case 'h':
			h_flag = SET;
			break;
		case 'p':
			p_flag = SET;
			port_literal = optarg;
			break;
		case'i':
			i_flag = SET;
			ipaddr_literal = optarg;
			break;
		default: /* '?' */
			err_flag = SET;
			break;
		}
	}
	
	if((h_flag == SET && (p_flag == SET || i_flag == SET)) || err_flag == SET) {
		if(h_flag == SET && (p_flag == SET || i_flag == SET)) {
			fprintf(stderr, "error: no other must be specified with "
					"option 'h'\n");
		}
		if(err_flag == SET) 
			fprintf(stderr, "error: invalid option specified\n");
		
		exit(1);
	}
	else if(h_flag == SET) {
		fprintf(stdout, "usage: %s [options]\n\n"
			"\t-h --help\tdisplay help\n"
			"\t-i --ip-address\tspecify ip address\n"
			"\t-p --port\tspecify port to listen on\n",
			argv[0]);
		exit(0);
	}
	else if(i_flag == NOT_SET && p_flag == NOT_SET) {
		fprintf(stderr, "error: no options specified\n");
		exit(1);
	}
	else if(i_flag == NOT_SET && p_flag == SET) {
		fprintf(stderr, "error: ip address to connect to was not specified\n");
		exit(1);
	}
	else if(i_flag == SET && p_flag == NOT_SET) {
		fprintf(stderr, "error: port to connect to was not specified\n");
		exit(1);
	}
	else { /* i_flag == SET && p_flag == SET */
		ip_address = ipaddr_literal;
		port = (uint16_t) atoi(port_literal);
	}
	
        int retval = initclient(&serverfd, ip_address, port);
        if(retval == -1) {
                fprintf(stderr, "error: server connection failed\n");
                exit(1);
        }

	/* start of test code */
        char buffer[] = "hello server";

        ssize_t bytes_written = write(serverfd, buffer, sizeof(buffer));
        if(bytes_written == 0 || bytes_written == -1) {
                fprintf(stderr, "no data to send\n");
                exit(1);
        }
        fprintf(stdout, "data sent\n");
	/* end of test code */

        exit(0);
}
