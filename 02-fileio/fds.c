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

	/* Just waiting for the user to hit return. */
	(void)getchar();

	printf("\nOpening /dev/zero...\n");
	if ((fd1 = open("/dev/zero", O_RDONLY)) < 0) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("fd1: %d\n", fd1);
	(void)getchar();

	printf("\nOpening /dev/zero a second time...\n");
	if ((fd2 = open("/dev/zero", O_RDONLY)) < 0) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("fd2: %d\n", fd2);
	(void)getchar();

	printf("\nNow closing fd1, but keeping fd2 open..\n");
	(void)close(fd1);

	printf("\nOpening /dev/zero a third time...\n");
	if ((fd3 = open("/dev/zero", O_RDONLY)) < 0) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("fd3: %d\n", fd3);
	(void)getchar();

	printf("\nNow closing fd2 and fd3.\n");
	(void)close(fd2);
	(void)close(fd3);

	printf("Now opening /dev/zero as a stream.\n");
	if ((f = fopen("/dev/zero", "r")) == NULL) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("f: %d\n\n", fileno(f));
	(void)fclose(f);

	printf("What happens if we close stderr and open a new file?\n");
	(void)getchar();
	(void)close(STDERR_FILENO);

	if ((fd3 = open("/dev/zero", O_RDONLY)) < 0) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("New fd: %d\n", fd3);
	(void)close(fd3);

	return EXIT_SUCCESS;
}
