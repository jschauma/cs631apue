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

int
main(void) {
	int flags, count, resid, loops;
	char buf[100000], *ptr;

	/* fill buffer with 'a' */
	memset(buf, 'a', 100000);

	/* set non-blocking mode on stdout */
	if ((flags = fcntl(STDOUT_FILENO, F_GETFL, 0)) < 0) {
		perror("getting file flags");
		exit(1);
	}
	if (fcntl(STDOUT_FILENO, F_SETFL, flags|O_NONBLOCK) < 0) {
		perror("setting file flags");
		exit(2);
	}

	for (loops = 0; loops < 50; loops++) {
		ptr = buf;
		resid = 100000;
		while(resid > 0) {
			count = write(STDOUT_FILENO, ptr, resid);
			if (count >= 0) {
				ptr += count;
				resid -= count;
				fprintf(stderr, "wrote %d bytes\n", count);
			} else {
				fprintf(stderr, "write error: %s\n",
						strerror(errno));
			}
		}
	}
	/* set file flags back as they were; not strictly necessary, since
	 * we exit right away, but illustrative of good practice */
	(void)fcntl(STDOUT_FILENO, F_SETFL, flags);
	exit(0);
}
