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
 * This program illustrates the usefulness of openat(2) to avoid
 * certain race conditions:
 *
 * Assume that you want to open the files "file1" and "file2" found
 * in the directory "../dir".  You might start out using
 *
 * fd1 = open("../dir/file1", ...)
 * fd2 = open("../dir/file2", ...)
 *
 * However, in between these two calls, another process might have
 * renamed the directory "../dir" to "../dir2", then created "../dir"
 * and placed a different file named "file2" into it.  Your program
 * now has opened two files from two different directories.
 *
 * With openat(2), you could guarantee that the call to open "file2"
 * will use the same directory as the call to open "file1":
 *
 * dfd = open("../dir/", O_DIRECTORY ...)
 * fd1 = openat(dfd, "file1", ...)
 * fd2 = openat(dfd, "file2", ...)
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
readData(int fd) {
	int n;
	char buf[BUFSIZ];
	bzero(buf, BUFSIZ);
	if ((n = read(fd, buf, BUFSIZ)) == -1) {
		(void)fprintf(stderr, "Unable to read data from fd %d: %s\n",
				fd, strerror(errno));
		exit(EXIT_FAILURE);
	}

	(void)printf("Read data: %s", buf);
}

void
readFileUsingOpenat(const int dfd, const char *f) {
	int fd;

	(void)printf("Opening '%s' using openat(2)...\n", f);
	if ((fd = openat(dfd, f, O_RDONLY)) == -1) {
		(void)fprintf(stderr, "Unable to open '%s': %s\n",
				f, strerror(errno));
		exit(EXIT_FAILURE);
	}

	readData(fd);
}

void
readFileUsingOpen(const char *f) {
	int fd;

	(void)printf("Opening '%s'...\n", f);

	if ((fd = open(f, O_RDONLY)) == -1) {
		(void)fprintf(stderr, "Unable to open '%s': %s\n",
				f, strerror(errno));
		exit(EXIT_FAILURE);
	}

	readData(fd);
}

void
reset() {
	system("rm -fr ../dir");
	system("mv ../dir2 ../dir");
}

void
makeFiles() {
}

void
setup() {
	/* Trivial setup via system(3) because we're lazy. */
	system("mkdir -p ./t ./dir");
	system("echo file1 > ./dir/file1");
	system("echo file2 > ./dir/file2");

	if (chdir("./t") < 0) {
		(void)fprintf(stderr, "Unable to cd to './t': %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}
}

void
simulateRaceCondition() {
	(void)printf("Simulating race condition by renaming '../dir'...\n");
	system("mv ../dir ../dir2");
	system("mkdir ../dir");
	system("echo notfile2 > ../dir/file2");
}

void
cleanup() {
	if (chdir("..") < 0) {
		(void)fprintf(stderr, "Unable to register exit handler: %s\n",
				strerror(errno));
	} else {
		system("rm -fr t dir dir2");
	}
}

int
main() {

	if (atexit(cleanup) < 0) {
		(void)fprintf(stderr, "Unable to register exit handler: %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	setup();

	readFileUsingOpen("../dir/file1");
	simulateRaceCondition();
	readFileUsingOpen("../dir/file2");

	reset();

	(void)printf("\nLet's try again...\n\n");

	int dfd;
	if ((dfd = open("../dir", O_RDONLY | O_DIRECTORY)) == -1) {
		(void)fprintf(stderr, "Unable to open directory '../dir': %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	readFileUsingOpenat(dfd, "file1");
	simulateRaceCondition();
	readFileUsingOpenat(dfd, "file2");

	return EXIT_SUCCESS;
}
