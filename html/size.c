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

/* This simple program displays the stat data relevant to file sizes. */

#include <sys/stat.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv) {
	struct stat sb;

	if (argc != 2) {
		fprintf(stderr, "usage: %s file\n", argv[0]);
		exit(1);
	}

	if (lstat(argv[1], &sb) == -1) {
		fprintf(stderr, "Can't stat %s: %s\n", argv[1],
						strerror(errno));
		exit(1);
	}

	printf("st_size   : %d\n", (int)sb.st_size);
	printf("st_blocks : %d\n", (int)sb.st_blocks);
	printf("st_blksize: %d\n", (int)sb.st_blksize);
	exit(0);
}
