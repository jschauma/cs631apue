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
 * This simple program creates a file with a hole in it, a so-called
 * sparse file.  Use hexdump(1) or od(1) to verify that there is no data
 * in between the two strings; then copy the file to to another and
 * observe that the kernel supplied zeros to fill the hole.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

#define BIGNUM 10240000

int
main(void) {
	int fd;

	if ((fd = creat("file.hole", S_IRUSR | S_IWUSR)) < 0) {
		perror("creat error");
		return EXIT_FAILURE;
	}

	if (write(fd, buf1, strlen(buf1)) != sizeof(buf1) - 1) {
		perror("error writing buf1");
		return EXIT_FAILURE;
	}

	if (lseek(fd, BIGNUM, SEEK_CUR) == -1) {
		perror("lseek error");
		return EXIT_FAILURE;
	}

	if (write(fd, buf2, strlen(buf2)) != sizeof(buf2) - 1) {
		perror("error writing buf2");
		return EXIT_FAILURE;
	}
	sleep(30);

	(void)close(fd);
	return EXIT_SUCCESS;
}
