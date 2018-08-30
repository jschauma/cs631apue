/*	$NetBSD: streamread.c,v 1.3 2003/08/07 10:30:50 agc Exp $
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
 *	@(#)streamread.c	8.1 (Berkeley) 6/8/93
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1

/*
 * This program creates a socket and then begins an infinite loop. Each time
 * through the loop it accepts a connection and prints out messages from it.
 * When the connection breaks, or a termination message comes through, the
 * program accepts a new connection.
 */

int main(int argc, char **argv)
{
	int sock;
	socklen_t length;
	struct sockaddr_storage server;
	int msgsock;
	char buf[1024];
	int rval;
	int domain, v4or6;
	void *s;
	socklen_t s_size;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s 4|6\n", argv[0]);
		exit(1);
	}
	v4or6 = atoi(argv[1]);
	if ((v4or6 != 4) && (v4or6 != 6)) {
		fprintf(stderr, "Usage: %s 4|6\n", argv[0]);
		exit(1);
	}

	domain = (v4or6 == 4) ? AF_INET : AF_INET6;

	/* Create socket */
	sock = socket(domain, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		exit(1);
	}
	/* Name socket using wildcards */

	if (domain == AF_INET) {
		struct sockaddr_in *sin = (struct sockaddr_in *)&server;
		sin->sin_family = AF_INET;
		sin->sin_addr.s_addr = INADDR_ANY;
		sin->sin_port = 0;
		s = sin;
		s_size = sizeof(*sin);
	} else {
		struct sockaddr_in6 *sin = (struct sockaddr_in6 *)&server;
		sin->sin6_family = AF_INET6;
		sin->sin6_addr = in6addr_any;
		sin->sin6_port = 0;
		s = sin;
		s_size = sizeof(*sin);
	}

	if (bind(sock, (struct sockaddr *)s, s_size)) {
		perror("binding stream socket");
		exit(1);
	}

	/* Find out assigned port number and print it out */
	length = sizeof(server);
	if (getsockname(sock, (struct sockaddr *)&server, &length)) {
		perror("getting socket name");
		exit(1);
	}

	if (domain == AF_INET) {
		struct sockaddr_in *s = (struct sockaddr_in *)&server;
		printf("Socket has port #%d\n", ntohs(s->sin_port));
	} else {
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&server;
		printf("Socket has port #%d\n", ntohs(s->sin6_port));
	}

	/* Start accepting connections */
	listen(sock, 5);
	do {
		msgsock = accept(sock, 0, 0);
		if (msgsock == -1)
			perror("accept");
		else do {
			bzero(buf, sizeof(buf));
			if ((rval = read(msgsock, buf, 1024)) < 0)
				perror("reading stream message");
			if (rval == 0)
				printf("Ending connection\n");
			else
				printf("-->%s\n", buf);
		} while (rval != 0);
		close(msgsock);
	} while (TRUE);
	/*
	 * Since this program has an infinite loop, the socket "sock" is
	 * never explicitly closed.  However, all sockets will be closed
	 * automatically when a process is killed or terminates normally.
	 */
	return 0;
}
