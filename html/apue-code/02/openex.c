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
 * This trivial program illustrates how to open files.  We present the
 * following use cases:
 *
 * - create a non-existent file
 * - "create" an existing file
 * - fail to create an existing file
 * - open an existing file
 * - fail to open a non-existing file
 * - truncate an existing file
 */

#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef SLEEP
#define SLEEP 10
#endif

void
createFile() {
	int fd;

	printf("Checking if './newfile' exists...\n");
	system("ls -l ./newfile");
	printf("Trying to create './newfile' with O_RDONLY | O_CREAT...\n");

	if ((fd = open("./newfile", O_RDONLY | O_CREAT,
					S_IRUSR | S_IWUSR)) == -1) {
		fprintf(stderr, "Unable to create './newfile': %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("'./newfile' created. File descriptor is: %d\n", fd);

	/* We are leaking a file descriptor here to illustrate that they
	 * are increasing as we open more files.  Normally, we would call
	 * close(2) whenever we're done using the decriptor. */
}

void
failExclFileCreation() {
	int fd;

	printf("Checking if './newfile' exists...\n");
	system("ls -l ./newfile");
	printf("Trying to create './newfile' with O_RDONLY | O_CREAT | O_EXCL...\n");

	if ((fd = open("./newfile", O_RDONLY | O_CREAT | O_EXCL,
					S_IRUSR | S_IWUSR)) == -1) {
		fprintf(stderr, "Unable to create './newfile': %s\n",
				strerror(errno));
	}

	/* We expect this to fail! */
	if (close(fd) == -1) {
		fprintf(stderr, "Closing failed: %s\n", strerror(errno));
	}
}


void
failOpenNonexistingFile() {
	int fd;

	printf("Trying to open (non-existant) './nosuchfile' with O_RDONLY...\n");

	if ((fd = open("./nosuchfile", O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to open './nosuchfile': %s\n",
				strerror(errno));
	}

	/* We know this is going to fail, but no need to complain. */
	(void)close(fd);
}

void
openFile() {
	int fd;

	printf("Trying to open './openex.c' with O_RDONLY...\n");

	if ((fd = open("./openex.c", O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to open './openex.c': %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("'./openex.c' opened. File descriptor is: %d\n", fd);

	if (close(fd) == 0) {
		printf("'./openex.c' closed again\n");
	}
}

void
truncateFile() {
	int fd;

	system("cp openex.c newfile");
	printf("Copied 'openex.c' to 'newfile'.\n");
	system("ls -l newfile");

	printf("Trying to open './newfile' with O_RDONLY | O_TRUNC...\n");

	if ((fd = open("./newfile", O_RDONLY | O_TRUNC)) == -1) {
		fprintf(stderr, "Unable to open './newfile': %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("'./newfile' opened. File descriptor is: %d\n", fd);
	printf("'./newfile' truncated -- see 'ls -l newfile'\n");
	system("ls -l newfile");

	(void)close(fd);
}



int
main() {
	createFile();
	system("ls -l newfile");
	printf("\n");
	sleep(SLEEP);

	createFile();
	system("ls -l newfile");
	printf("\n");
	sleep(SLEEP);

	failExclFileCreation();
	printf("\n");
	sleep(SLEEP);

	openFile();
	printf("\n");
	sleep(SLEEP);

	failOpenNonexistingFile();
	printf("\n");
	sleep(SLEEP);

	truncateFile();

	return 0;
}
