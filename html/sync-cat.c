/* simple-cat.c
 * Stripped down version of 'cat'.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 512

int
main(void)
{
	int n;
	char buf[BUFFSIZE];
	int flags;

	if ((flags = fcntl(STDOUT_FILENO, F_GETFL, 0)) < 0) {
		perror("Can't get file descriptor flags");
		exit(1);
	}

	flags |= O_SYNC;

	if (fcntl(STDOUT_FILENO, F_SETFL, flags) < 0) {
		perror("Can't set file descriptor flags");
		exit(1);
	}

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0 )
		if ( write(STDOUT_FILENO, buf, n) != n ) {
			fprintf(stderr, "write error\n");
			exit(1);
		}

	if (n < 0) {
		fprintf(stderr, "read error\n");
		exit(1);
	}

	exit(0);
}
