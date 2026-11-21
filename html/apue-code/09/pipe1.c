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

/* A simple example of IPC using a pipe(2).  This may
 * be compared to
 *
 * echo msg | cat
 *
 * but we display information in each process about
 * its pid and ppid.
 *
 * Depending on the order in which the parent and
 * child end up running, the child may report a ppid
 * of '1': if the parent has exited before the child
 * has finished reading data, then init(8) will have
 * reaped (and possibly reparented) it.
 */

#include <sys/wait.h>

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main() {
	int n, r, fd[2];
	pid_t pid;
	char line[BUFSIZ];

	if (pipe(fd) < 0) {
		err(EXIT_FAILURE, "pipe");
		/* NOTREACHED */
	}

	if ((pid = fork()) < 0) {
		err(EXIT_FAILURE, "fork");
		/* NOTREACHED */
	} else if (pid > 0) {
		/* The parent closes the read-end of
		 * the pipe, since it only writes data
		 * to the child. */
		(void)close(fd[0]);
		(void)printf("P=> Parent process with pid %d (and its ppid %d).\n",
				getpid(), getppid());
		(void)printf("P=> Sending a message to the child process (pid %d).\n", pid);
		(void)snprintf(line, BUFSIZ, "Hello child!  I'm your parent, pid %d!\n",
				getpid());
		if ((r = write(fd[1], line, strlen(line))) < 0) {
			(void)fprintf(stderr, "Unable to write to pipe: %s\n",
					strerror(errno));
		}
		(void)close(fd[1]);
	} else {
		/* The child process closes the write
		 * end of the pipe, since it only
		 * plans on reading. */
		(void)close(fd[1]);
		(void)printf("C=> Child process with pid %d (and its ppid %d).\n",
				getpid(), getppid());
		(void)printf("C=> Reading a message from the parent (pid %d):\n",
				getppid());
		if ((n = read(fd[0], line, BUFSIZ)) < 0) {
			(void)fprintf(stderr, "Unable to read from pipe: %s\n",
					strerror(errno));
		} else if ((r = write(STDOUT_FILENO, line, n)) < 0) {
			(void)fprintf(stderr, "Unable to write to pipe: %s\n",
					strerror(errno));
		}
		(void)close(fd[0]);
	}

	(void)wait(NULL);
	return EXIT_SUCCESS;
}
