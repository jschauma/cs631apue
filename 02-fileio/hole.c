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

	/* Note: we're comparing a signed to an unsigned size_t;
	 * without the cast, your compiler would warn you about this.
	 * See e.g. https://is.gd/RCpnlr for a discussion of why this
	 * warning is needed.  In _this_ case, we happen to know
	 * that the length of the buffer is never larger than the
	 * max value of a size_t, so we can safely cast it.  This
	 * is not always the case! */
	if (write(fd, buf1, strlen(buf1)) != (ssize_t)strlen(buf1)) {
		perror("error writing buf1");
		return EXIT_FAILURE;
	}

	if (lseek(fd, BIGNUM, SEEK_CUR) == -1) {
		perror("lseek error");
		return EXIT_FAILURE;
	}

	if (write(fd, buf2, strlen(buf2)) != (ssize_t)strlen(buf2)) {
		perror("error writing buf2");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
