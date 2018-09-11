/*
 * A program to show the value of some file
 * descriptors.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main() {
	int fd1, fd2, fd3;
	FILE *f;

	printf("STDIN_FILENO: %d\n", STDIN_FILENO);
	printf("stdin: %d\n", fileno(stdin));
	printf("STDOUT_FILENO: %d\n", STDOUT_FILENO);
	printf("stdout: %d\n", fileno(stdout));
	printf("STDERR_FILENO: %d\n", STDERR_FILENO);
	printf("stderr: %d\n", fileno(stderr));

	printf("\nOpening /dev/zero...\n");
	if ((fd1 = open("/dev/zero", O_RDONLY)) < 0) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
	} else {
		printf("fd1: %d\n", fd1);
	}

	printf("\nOpening /dev/zero a second time...\n");
	if ((fd2 = open("/dev/zero", O_RDONLY)) < 0) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("fd2: %d\n", fd2);

	printf("\nNow closing fd1, but keeping fd2 open..\n");
	(void)close(fd1);

	printf("\nOpening /dev/zero a third time...\n");
	if ((fd3 = open("/dev/zero", O_RDONLY)) < 0) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("fd3: %d\n", fd3);

	printf("\nNow closing fd2 and fd3.\n");
	(void)close(fd2);
	(void)close(fd3);

	printf("Now opening /dev/zero as a stream.\n");
	if ((f = fopen("/dev/zero", "r")) == NULL) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("f: %d\n", fileno(f));
	(void)fclose(f);

	return EXIT_SUCCESS;
}
