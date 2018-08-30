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
main(int argc, char **argv) {
	int fd, fd2, fd3;
	FILE *f;

	printf("STDIN_FILENO: %s\n", STDIN_FILENO);
	printf("stdin: %d\n", fileno(stdin));
	printf("STDOUT_FILENO: %d\n", STDOUT_FILENO);
	printf("stdout: %d\n", fileno(stdout));
	printf("STDERR_FILENO: %d\n", STDERR_FILENO);
	printf("stderr: %d\n", fileno(stderr));

	if ((fd = open("/dev/zero", O_RDONLY)) < 0) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
	} else {
		printf("fd: %d\n", fd);
	}

	if ((fd2 = open("/dev/zero", O_RDONLY)) < 0) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("fd: %d\n", fd2);

	(void)close(fd);

	if ((fd3 = open("/dev/zero", O_RDONLY)) < 0) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("fd: %d\n", fd3);

	(void)close(fd2);
	(void)close(fd3);

	if ((f = fopen("/dev/zero", "r")) == NULL) {
		fprintf(stderr, "Unable to open /dev/zero: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("f: %d\n", fileno(f));
	(void)fclose(f);

	return EXIT_SUCCESS;
}
