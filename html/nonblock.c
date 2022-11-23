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
 * This program uses nonblocking I/O to write 50 megabytes of data to
 * its standard output, reporting on standard error the number of bytes
 * written by each system call.  If standard output is a pipe or network,
 * connection, we may observe that some write system calls "fail", because
 * buffers are full, or flow-control has been applied; this is normal.
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 1024000

int
main(int argc, char **argv) {
	int flags, count, loops, num;
	char buf[BUFSIZE], *ptr;

	(void)argv;

	/* fill buffer with 'a' */
	memset(buf, 'a', BUFSIZE);

	if ((flags = fcntl(STDOUT_FILENO, F_GETFL, 0)) < 0) {
		perror("getting file flags");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if (argc > 1) {
		/* set non-blocking mode on stdout */
		if (fcntl(STDOUT_FILENO, F_SETFL, flags|O_NONBLOCK) < 0) {
			perror("setting file flags");
			exit(EXIT_FAILURE);
			/* NOTREACHED */
		}
	}

	for (loops = 0; loops < 50; loops++) {
		ptr = buf;
		num = BUFSIZE;
		while(num > 0) {
			count = write(STDOUT_FILENO, ptr, num);
			if (count >= 0) {
				ptr += count;
				num -= count;
				(void)fprintf(stderr, "wrote %d bytes\n", count);
			} else {
				(void)fprintf(stderr, "write error: %s\n",
						strerror(errno));
			}
		}
	}
	/* set file flags back as they were; not strictly necessary, since
	 * we exit right away, but illustrative of good practice */
	(void)fcntl(STDOUT_FILENO, F_SETFL, flags);
	exit(EXIT_SUCCESS);
}
