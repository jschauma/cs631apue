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

/* This program illustrates two things:
 * - the fact that after a fork both parent and child are (mostly)
*    identical; that is, the printf(3) buffer, if not yet flushed, is
*    copied into the child
*  - the difference between unbuffered and buffered I/O
*
*  Illustrate by running twice, once with stdout connected to a terminal
*  (line-buffered) and once to a pipe or a file.
*/

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global = 0;
char buf[] = "a write to stdout\n";

int
main() {
	int local;
	pid_t pid;

	local = 1;
	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1) {
		err(EXIT_FAILURE, "write error");
		/* NOTREACHED */
	}
	(void)printf("before fork\n");

	if ((pid = fork()) < 0) {
		err(EXIT_FAILURE, "fork error");
		/* NOTREACHED */
	} else if (pid == 0) {	/* child */
		global++;
		local++;
	} else {		/* parent */
		sleep(1);
		global--;
		local--;
	}

	(void)printf("pid = %d, ppid = %d, global = %d, local = %d\n",
			getpid(), getppid(), global, local);
	return EXIT_SUCCESS;
}
