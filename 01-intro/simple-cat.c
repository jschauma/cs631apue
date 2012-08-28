/*
 * Stripped down version of 'cat', using unbuffered I/O.
 * ./simple-cat < simple-cat.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 32768

int
main(int argc, char **argv) {
	int n;
	char buf[BUFFSIZE];

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			fprintf(stderr, "write error\n");
			exit(1);
		}
	}

	if (n < 0) {
		fprintf(stderr, "read error\n");
		exit(1);
	}

	return(0);
}
