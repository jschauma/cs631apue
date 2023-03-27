/* This file is part of the sample code and exercises
 * used by the class "Advanced Programming in the UNIX
 * Environment" taught by Jan Schaumann
 * <jschauma@netmeister.org> at Stevens Institute of
 * Technology.
 *
 * https://stevens.netmeister.org/631/
 */

/*	$NetBSD: dgramsend.c,v 1.3 2003/08/07 10:30:50 agc Exp $
 *
 * Copyright (c) 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)dgramsend.c	8.1 (Berkeley) 6/8/93
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* 'Dover Beach' by Matthew Arnold -- look it up. */
#define DATA "The sea is calm tonight, the tide is full . . ."

/*
 * Here I send a datagram to a receiver whose name I get from the command
 * line arguments.  The form of the command line is dgramsend hostname
 * portnumber
 */

int main(int argc, char **argv)
{
	int sock, port;
	struct sockaddr_in name;
	struct hostent *hp;

	memset(&name, 0, sizeof(name));

	if (argc != 3) {
		(void)printf("Usage: %s hostname port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	port = atoi(argv[2]);
	if ((port < 1) || (port > 65536)) {
		(void)fprintf(stderr, "Invalid port: %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("opening datagram socket");
		exit(EXIT_FAILURE);
	}

	/*
	 * Construct name, with no wildcards, of the socket to send to.
	 * getnostbyname() returns a structure including the network address
	 * of the specified host.  The port number is taken from the command
	 * line.
	 */
	if ((hp = gethostbyname(argv[1])) == 0) {
		(void)fprintf(stderr, "%s: unknown host\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	bcopy(hp->h_addr, &name.sin_addr, hp->h_length);
	name.sin_family = PF_INET;
	name.sin_port = htons(port);

	if (sendto(sock, DATA, sizeof(DATA), 0,
	    (struct sockaddr *)&name, sizeof(name)) < 0) {
		perror("sending datagram message");
	}
	(void)close(sock);
	return EXIT_SUCCESS;
}
