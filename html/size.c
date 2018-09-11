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
