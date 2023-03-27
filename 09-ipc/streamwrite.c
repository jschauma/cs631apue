/* This file is part of the sample code and exercises
 * used by the class "Advanced Programming in the UNIX
 * Environment" taught by Jan Schaumann
 * <jschauma@netmeister.org> at Stevens Institute of
 * Technology.
 *
 * https://stevens.netmeister.org/631/
 */

/*	$NetBSD: streamwrite.c,v 1.3 2003/08/07 10:30:50 agc Exp $
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
 *	@(#)streamwrite.c	8.1 (Berkeley) 6/8/93
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* http://poetry.eserver.org/light-brigade.html */
#define DATA "Half a league, half a league . . ."

/*
 * This program creates a socket and initiates a connection with the socket
 * given in the command line.  One message is sent over the connection and
 * then the socket is closed, ending the connection. The form of the command
 * line is streamwrite hostname portnumber
 */

int main(int argc, char **argv)
{
	int sock, port;
	struct sockaddr_in6 server;
	struct hostent *hp;

	memset(&server, 0, sizeof(server));

	if (argc != 3) {
		(void)printf("Usage: %s hostname port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	port = atoi(argv[2]);
	if ((port < 1) || (port > 65536)) {
		(void)fprintf(stderr, "Invalid port: %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	if ((sock = socket(PF_INET6, SOCK_STREAM, 0)) < 0) {
		perror("opening stream socket");
		exit(EXIT_FAILURE);
	}

	server.sin6_family = PF_INET6;
	if ((hp = gethostbyname2(argv[1], PF_INET6)) == NULL) {
		(void)fprintf(stderr, "%s: unknown host\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	bcopy(hp->h_addr, &server.sin6_addr, hp->h_length);
	server.sin6_port = htons(port);

	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("connecting stream socket");
		exit(EXIT_FAILURE);
	}
	if (write(sock, DATA, sizeof(DATA)) < 0)
		perror("writing on stream socket");
	(void)close(sock);
	return EXIT_FAILURE;
}
