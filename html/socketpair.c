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

/*	$NetBSD: socketpair.c,v 1.3 2003/08/07 10:30:50 agc Exp $
 *
 * Copyright (c) 1986, 1993
 * 	The Regents of the University of California.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 *	@(#)socketpair.c	8.1 (Berkeley) 6/8/93
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DATA1 "In Xanadu, did Kubla Khan . . ."
#define DATA2 "A stately pleasure dome decree . . ."

/*
 * This program creates a pair of connected sockets then forks and
 * communicates over them.  This is very similar to communication with pipes,
 * however, socketpairs are two-way communications objects. Therefore I can
 * send messages in both directions.
 */

int
main() {
	int sockets[2], child;
	char buf[BUFSIZ];

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) {
		err(EXIT_FAILURE, "opening stream socket pair");
		/* NOTREACHED */
	}

	/* Note: Execution order of parent/child is not guaranteed!
	 * We're forcing an order by having the parent read(2),
	 * meaning it blocks until the child process has sent data,
	 * but there is no guarantee that the parent would necessarily
	 * start executing first. */

	if ((child = fork()) == -1) {
		err(EXIT_FAILURE, "fork");
		/* NOTREACHED */
	} else if (child) {
		pid_t pid = getpid();
		close(sockets[0]);
		printf("Parent (%d) reading...\n", pid);
		if (read(sockets[1], buf, BUFSIZ) < 0) {
			err(EXIT_FAILURE, "reading stream message");
			/* NOTREACHED */
		}
		printf("Parent (%d) read: \"%s\"\n", pid, buf);

		printf("Parent (%d) sending...\n", pid);
		if (write(sockets[1], DATA2, sizeof(DATA2)) < 0) {
			err(EXIT_FAILURE, "writing stream message");
			/* NOTREACHED */
		}
		printf("Parent (%d) sent: \"%s\"\n", pid, DATA2);
		close(sockets[1]);
	} else {
		close(sockets[1]);
		pid_t pid = getpid();
		printf("Child  (%d) sending...\n", pid);
		if (write(sockets[0], DATA1, sizeof(DATA1)) < 0) {
			err(EXIT_FAILURE, "writing stream message");
			/* NOTREACHED */
		}
		printf("Child  (%d) sent: \"%s\"\n", pid, DATA1);

		printf("Child  (%d) reading...\n", pid);
		if (read(sockets[0], buf, BUFSIZ) < 0) {
			err(EXIT_FAILURE, "reading stream message");
			/* NOTREACHED */
		}
		printf("Child  (%d) read: \"%s\"\n", pid, buf);
		close(sockets[0]);
	}
	(void)wait(NULL);
	return EXIT_SUCCESS;
}
