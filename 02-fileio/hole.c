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
#include <unistd.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int
main(void) {
	int fd;

	if ((fd = creat("file.hole", S_IRUSR | S_IWUSR)) < 0) {
		perror("creat error");
		return 1;
	}

	if (write(fd, buf1, 10) != 10) {
		perror("error writing buf1");
		return 1;
	}

	if (lseek(fd, 10240000, SEEK_CUR) == -1) {
		perror("lseek error");
		return 1;
	}

	if (write(fd, buf2, 10) != 10) {
		perror("error writing buf2");
		return 1;
	}

	exit(EXIT_SUCCESS);
}
