/* This file is part of the sample code and exercises
 * used by the class "Advanced Programming in the UNIX
 * Environment" taught by Jan Schaumann
 * <jschauma@netmeister.org> at Stevens Institute of
 * Technology.
 *
 * This file is in the public domain.
 *
 * You don't have to, but if you feel like
 * acknowledging where you got this code, you may
 * reference me by name, email address, or point
 * people to the course website:
 * https://stevens.netmeister.org/631/
 */

/* This program illustrates the uselessness of an external cd(1) command,
 * as required by POSIX.1; note that many Unix versions don't bother
 * implementing /usr/bin/cd anymore, but FreeBSD and Mac OS X, for
 * example, still provide it via a generic shell builtin wrapper:
 * http://www.freebsd.org/cgi/cvsweb.cgi/~checkout~/src/usr.bin/alias/generic.sh?rev=HEAD;content-type=text%2Fplain
 */
#include <sys/param.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char **argv) {
	char buf[MAXPATHLEN];

	if (argc != 2) {
		fprintf(stderr, "%s: need a directory!\n", argv[0]);
		return(EXIT_FAILURE);
	}

	if (chdir(argv[1]) == -1) {
		fprintf(stderr, "%s: unable to chdir to %s: %s\n", argv[0],
						argv[1], strerror(errno));
		return(EXIT_FAILURE);
	}

	printf("Ok, cwd is now: %s\n", getcwd(buf, MAXPATHLEN));

	exit(EXIT_SUCCESS);
}
