/* This file is part of the sample code and exercises
 * used by the class "Advanced Programming in the UNIX
 * Environment" taught by Jan Schaumann
 * <jschauma@netmeister.org> at Stevens Institute of
 * Technology.
 *
 * https://stevens.netmeister.org/631/
 */


/* This program illustrates the use of the various
 * 'getpw*' functions.  If given a username or a uid,
 * it will attempt to print the information for the
 * given user, otherwise, it will iterate over all
 * entries and print them out in /etc/passwd format.
 *
 * ./a.out 0
 * ./a.out root
 * ./a.out
 *
 * Btw, what happens when you run this program as root?
 */

/* We pulled in some code from id(1):
 *
 * Copyright (c) 1991, 1993
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
 */
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct passwd *who(char *);

void printAllEntries(void);
void printOneStruct(const struct passwd *pw);

struct passwd *
who(char *u)
{
	struct passwd *pw;
	long id;
	char *ep;

	/* Translate user argument into a pw pointer.
	 * First, try to get it as specified.  If that
	 * fails, try it as a number. */
	if ((pw = getpwnam(u)) != NULL) {
		return pw;
	}

	id = strtol(u, &ep, 10);
	if (*u && !*ep && (pw = getpwuid(id))) {
		return pw;
	}

	errx(EXIT_FAILURE, "%s: No such user", u);
	/* NOTREACHED */
	return NULL;
}

void
printAllEntries(void) {
	struct passwd *pw;
	errno = 0;
	while ((pw = getpwent()) != NULL) {
		printOneStruct(pw);
	}
	if (errno) {
		fprintf(stderr, "%s: Unable to call getpwent(3): %s\n",
				getprogname(), strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* We're exiting after this call, so the
	 * kernel would close the password database
	 * for us, but since we want to develop
	 * good coding habits, we're calling
	 * endpwent(3) here just like we'd call
	 * close(2) in other programs. */
	endpwent();
}

void
printOneStruct(const struct passwd *pw) {
	assert(pw);
	printf("%s:%s:%d:%d:%s:%s:%s\n", pw->pw_name, pw->pw_passwd,
					pw->pw_uid, pw->pw_gid,
					pw->pw_gecos, pw->pw_dir,
					pw->pw_shell);
}

int
main(int argc, char ** argv)
{
	(void)setprogname(argv[0]);

	if (argc > 2) {
		fprintf(stderr, "Usage: %s [name|uid]\n", getprogname());
		exit(EXIT_FAILURE);
	}

	if (argc == 2) {
		printOneStruct(who(argv[1]));
	} else {
		printAllEntries();
	}

	return EXIT_SUCCESS;
}
