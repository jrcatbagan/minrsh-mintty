/*
 * File: options.c
 * Created: 2015, May 16
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
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <common/network.h>
#include <common/options.h>
#include <common/defines.h>

int extract_options(struct net_info_t *net_info, int argc, char **argv)
{
	int retoption;
	char *port_literal, *ipaddr_literal;
	unsigned char flags = 0x00;

	/* valid options for minrsh/minrshd */
	struct option long_options[] = 
	{
		{"help", no_argument, NULL, 'h'},
		{"ip-address", required_argument, NULL, 'i'},
		{"port", required_argument, NULL, 'p'},
		{0, 0, 0, 0},
	};

	/* disable getopt_long from printing error messages */
	opterr = 0;

	while((retoption = getopt_long(argc, argv, "hi:p:", 
				       long_options, NULL)) != -1) {
		switch(retoption) {
		case 'h':
			flags |= H_FLAG_SET;
			break;
		case 'p':
			flags |= P_FLAG_SET;
			port_literal = optarg;
			break;
		case'i':
			flags |= I_FLAG_SET;
			ipaddr_literal = optarg;
			break;
		default: /* '?' */
			flags |= ERR_FLAG_SET;
			break;
		}
	}

	if (flags & ERR_FLAG_SET) {
		fprintf(stderr, "error: invalid option specified\n");
		return -1;
	}

	/* we reach this point assuming that no errors have occured; i.e. no invalid option was
	   specified on the command-line */
	if ((flags & H_FLAG_SET) && (flags & ~H_FLAG_SET)) {
		fprintf(stderr, "error: no other options must be specified with the 'help' option\n");
		return -1;
	}
	else if ((flags & H_FLAG_SET) && !(flags & ~H_FLAG_SET)) {
		fprintf(stdout, "usage: %s [options]\n\n"
				"\t-h --help\tdisplay help\n"
				"\t-i --ip-address\t specify IP address\n"
				"\t-p --port\tspecify port\n",
				argv[0]);
		exit(EXIT_SUCCESS);
	}
	else {
		if ((flags & P_FLAG_SET) && (flags & I_FLAG_SET)) {
			/* both the port number and the IP address have been specified */
			net_info->ip_address = ipaddr_literal;
			net_info->port = (uint16_t) atoi(port_literal);

			printf("%s - %d\n", net_info->ip_address, net_info->port);
		}
		else {
			/* one of the network parameters is missing */
			fprintf(stderr, "error: incomplete network parameters\n");
			return -1;
		}
	}
	
	return 0;
}
