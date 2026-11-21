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
 * This program creates a new file, writes ten bytes to it
 * and then seeks to the end of the file.  Where do we end up?
 */

#include <sys/stat.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DATA "0123456789"

void
printOffset(int fd, int whence) {
	off_t off;

	if ((off = lseek(fd, 0, whence)) == -1) {
		err(EXIT_FAILURE, "Unable to lseek");
		/* NOTREACHED */
	}
	(void)printf("Offset: %d\n", (int)off);
}

int
main(void) {
	int fd;
	int len;

	if ((fd = open("f", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR)) == -1) {
		err(EXIT_FAILURE, "Unable create file 'f'");
		/* NOTREACHED */
	}

	printOffset(fd, SEEK_CUR);

	len = sizeof(DATA) - 1;
	if (write(fd, DATA, len) != len) {
		err(EXIT_FAILURE, "Unable write");
		/* NOTREACHED */
	}

	printOffset(fd, SEEK_CUR);
	printOffset(fd, SEEK_END);
	(void)close(fd);

	if (unlink("f") < 0) {
		err(EXIT_FAILURE, "Unable to unlink 'f'");
		/* NOTREACHED */
	}

	return EXIT_SUCCESS;
}
