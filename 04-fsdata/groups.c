/* This program is a trivial version of the 'groups'
 * command.
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
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct passwd *who(char *);

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

int
main(int argc, char ** argv) {
	gid_t *groups, *glist;
	int cnt, maxgroups, ngroups;
	struct passwd *pw;
	struct group *gr;

	(void)setprogname(argv[0]);

	if (argc > 2) {
		(void)fprintf(stderr, "Usage: %s [name|uid]\n", getprogname());
		exit(EXIT_FAILURE);
	}

	pw = argv[1] ? who(argv[1]) : NULL;

	if ((maxgroups = sysconf(_SC_NGROUPS_MAX)) < 0) {
		err(EXIT_FAILURE, "unable to call sysconf");
		/* NOTREACHED */
	}
	if ((groups = malloc((maxgroups + 1) * sizeof(gid_t))) == NULL) {
		err(EXIT_FAILURE, "unable to malloc");
		/* NOTREACHED */
	}
	glist = groups;

	if (pw) {
		ngroups = maxgroups;
		if (getgrouplist(pw->pw_name, pw->pw_gid, glist, &ngroups)
				== -1) {
			if ((glist = malloc(ngroups * sizeof(gid_t))) == NULL) {
				err(EXIT_FAILURE, "unable to malloc");
				/* NOTREACHED */
			}
			(void)getgrouplist(pw->pw_name, pw->pw_gid, glist,
					&ngroups);
		}
	} else {
		glist[0] = getgid();
		if ((ngroups = getgroups(maxgroups, glist + 1) + 1) == 0) {
			err(EXIT_FAILURE, "unable to getgroups");
			/* NOTREACHED */
		}
	}

	for (cnt = 0; cnt < ngroups; ++cnt) {
		if (cnt && glist[cnt] == glist[0]) {
			continue;
		}
		if ((gr = getgrgid(glist[cnt])) == NULL) {
			(void)fprintf(stderr, "%s: unable to getgrgid(%u): %s\n",
					getprogname(), glist[cnt], strerror(errno));
		} else {
			(void)printf("%s", gr->gr_name);
			if (cnt < ngroups) {
				(void)printf(" ");
			}
		}
	}
	if (cnt) {
		(void)printf("\n");
	}

	if (glist != groups) {
		(void)free(glist);
	}
	(void)free(groups);

	return EXIT_SUCCESS;
}
