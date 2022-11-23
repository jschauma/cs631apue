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

/* simple-cat.c, O_SYNC version
 *
 * Use this program to illustrate the use of fcntl(2)
 * to set the O_SYNC flag on STDOUT, which we do not
 * open ourselves, so can't set that flag on at that
 * time.
 *
 * This also illustrates the difference between
 * synchronous and asynchronous I/O:
 *
 * Create a large enough file:
 *
 * dd if=/dev/zero of=file bs=$((1024 * 1024)) count=25
 *
 * Then run this program:
 *
 * time ./a.out <file >out
 *
 * Then, comment out the setting of O_SYNC, recompile
 * the program and run it again.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 512

int
main(int argc, char **argv) {
	int n;
	char buf[BUFFSIZE];
	int flags;

	/* cast to void to silence compiler warnings */
	(void)argc;
	(void)argv;

	if ((flags = fcntl(STDOUT_FILENO, F_GETFL, 0)) < 0) {
		perror("Can't get file descriptor flags");
		exit(EXIT_FAILURE);
	}

	//flags |= O_SYNC;

	if (fcntl(STDOUT_FILENO, F_SETFL, flags) < 0) {
		perror("Can't set file descriptor flags");
		exit(EXIT_FAILURE);
	}

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0 )
		if ( write(STDOUT_FILENO, buf, n) != n ) {
			perror("write error");
			exit(EXIT_FAILURE);
		}

	if (n < 0) {
		perror("read error");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
