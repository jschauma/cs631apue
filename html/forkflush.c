/* This program illustrates two things:
 * - the fact that after a fork both parent and child are (mostly)
*    identical; that is, the printf(3) buffer, if not yet flushed, is
*    copied into the child
*  - the difference between unbuffered and buffered I/O
*
*  Illustrate by running twice, once with stdout connected to a terminal
*  (line-buffered) and once to a pipe or a file.
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int glob = 6;
char buf[] = "a write to stdout\n";

int
main(int argc, char **argv) {
	int var;
	pid_t pid;

	var = 88;
	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1) {
		fprintf(stderr, "%s: write error: %s\n",
				argv[0], strerror(errno));
		exit(1);
	}
	printf("before fork\n");

	if ((pid = fork()) < 0) {
		fprintf(stderr, "%s: fork error: %s\n",
				argv[0], strerror(errno));
		exit(1);
	} else if (pid == 0) { /* child */
		glob++;
		var++;
	} else {		/* parent */
		sleep(2);
	}

	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	exit(0);
}
