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

/*
 * Stripped down version of 'cat', using unbuffered I/O.
 * ./simple-cat < simple-cat.c
 *
 * Guess what, this is also a primitive version of 'cp':
 * ./simple-cat <simple-cat.c >simple-cat.copy
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef BUFFSIZE
#define BUFFSIZE 32768
#endif

int
main(int argc, char **argv) {
	int n;
	char buf[BUFFSIZE];

	/* cast to void to silence compiler warnings */
	(void)argc;
	(void)argv;

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			fprintf(stderr, "Unable to write: %s\n",
strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	if (n < 0) {
		fprintf(stderr, "Unable to read: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	return(EXIT_SUCCESS);
}
