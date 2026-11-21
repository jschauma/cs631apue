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

#include <sys/stat.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef SLEEP
#define SLEEP 3
#endif

int
main(int argc, char **argv) {
	struct stat sb;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file>\n", argv[0]);
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if (stat(argv[1], &sb) < 0) {
		err(EXIT_FAILURE, "stat");
		/* NOTREACHED */
	}

	(void)printf("%d\n", (int)sb.st_atime);
	return EXIT_SUCCESS;
}
