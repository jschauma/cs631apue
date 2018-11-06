/* A simple example of IPC using a pipe(2).  This may be compared to
 *
 * echo msg | cat
 *
 * but we display information in each process about its pid and ppid.
 *
 * Depending on the order in which the parent and child end up running,
 * the child may report a ppid of '1': if the parent has exited before the
 * child has finished reading data, then init(8) will have reaped (and
 * possibly reparented) it.
 */

#include <sys/wait.h>

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
		perror("pipe error");
		exit(EXIT_FAILURE);
	}

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(EXIT_FAILURE);
	} else if (pid > 0) {		/* parent */
		close(fd[0]);
		printf("P=> Parent process with pid %d (and its ppid %d).\n",
				getpid(), getppid());
		printf("P=> Sending a message to the child process (pid %d):\n", pid);
		snprintf(line, BUFSIZ, "Hello child!  I'm your parent pid %d!\n",
				getpid());
		if ((r = write(fd[1], line, strlen(line))) < 0) {
			fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
		}
		close(fd[1]);

	} else {			/* child */
		close(fd[1]);
		printf("C=> Child process with pid %d (and its ppid %d).\n",
				getpid(), getppid());
		printf("C=> Reading a message from the parent (pid %d):\n", getppid());
		n = read(fd[0], line, BUFSIZ);
		close(fd[0]);
		if ((r = write(STDOUT_FILENO, line, n)) < 0) {
			fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
		}
	}

	(void)wait(NULL);
	return EXIT_SUCCESS;
}
