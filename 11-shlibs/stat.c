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
