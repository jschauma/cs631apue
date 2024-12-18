/* This file is part of the sample code and exercises
 * used by the class "Advanced Programming in the UNIX
 * Environment" taught by Jan Schaumann
 * <jschauma@netmeister.org> at Stevens Institute of
 * Technology.
 *
 * https://stevens.netmeister.org/631/
 *
 * This file is derived from the IPC tutorials
 * provided by your NetBSD system under
 * /usr/share/doc/.
 */

/*	$NetBSD: udgramsend.c,v 1.3 2003/08/07 10:30:50 agc Exp $
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
 *	@(#)udgramsend.c	8.1 (Berkeley) 6/8/93
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <err.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* 'Dover Beach' by Matthew Arnold -- look it up. */
#define DATA "The sea is calm tonight, the tide is full . . ."

/*
 * Here I send a datagram to a receiver whose name I get from the command
 * line arguments.  The form of the command line is udgramsend pathname
 */

int
main(int argc, char **argv) {
	int sock;
	struct sockaddr_un name;

	if (argc != 2) {
		(void)fprintf(stderr, "usage: send <socket>\n");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if ((sock = socket(PF_LOCAL, SOCK_DGRAM, 0)) < 0) {
		err(EXIT_FAILURE, "opening datagram socket");
		/* NOTREACHED */
	}

	name.sun_family = PF_LOCAL;
	(void)strncpy(name.sun_path, argv[1], sizeof(name.sun_path));

	if (sendto(sock, DATA, sizeof(DATA), 0,
	    (struct sockaddr *)&name, sizeof(struct sockaddr_un)) < 0) {
		err(EXIT_FAILURE, "sending datagram message");
		/* NOTREACHED */
	}
	(void)close(sock);
	return EXIT_SUCCESS;
}
