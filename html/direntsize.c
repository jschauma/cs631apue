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

/* This program prints out the amount of space
 * required to represent a directory entry in the Unix
 * File System per <ufs/ufs/dir.h>.
 *
 * ./a.out filename
 *
 * You should be able to determine how many files of a
 * given filename length you can create in a directory
 * without that directory size increasing.
 *
 * Note that once a directory size has increased,
 * removing files from it will not shrink it again
 * (on UFS/FFS).
 */

#include <sys/types.h>

#include <ufs/ufs/dir.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s name\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	printf("%ld\n", UFS_DIRECTSIZ(strlen(argv[1])));
	exit(EXIT_SUCCESS);
}
