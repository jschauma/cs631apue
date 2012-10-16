/* A simple example of IPC using a pipe(2).  This may be compared to
 *
 * echo msg | cat
 *
 * but we display information in each process about its pid and ppid.
 *
 * If we don't pass "-w", then the parent exits, and our child process
 * will show that it was reaped by init(8) (ie pid 1), since the child
 * would otherwise necessarily complete after the parent, as it's blocked
 * on reading from the pipe.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int optind;

int
main(int argc, char **argv) {
	int ch, n, fd[2];
	pid_t pid;
	char line[64];
	int parent_waits = 0;

	while ((ch = getopt(argc, argv, "w")) != -1) {
		switch(ch) {
			case 'w':
				parent_waits = 1;
				break;
		}
	}
	argc -= optind;
	argv += optind;

	if (pipe(fd) < 0) {
		perror("pipe error");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid > 0) {		/* parent */
		close(fd[0]);
		printf("Parent process with pid %d (and its ppid %d).\n",
				getpid(), getppid());
		printf("Sending a message to the child process (pid %d):\n", pid);
		snprintf(line, 64, "Hello child!  I'm your parent pid %d!\n",
				getpid());
		write(fd[1], line, strlen(line));
		close(fd[1]);

		if (parent_waits)
			wait(NULL);
	} else {			/* child */
		close(fd[1]);
		printf("Child process with pid %d (and its ppid %d).\n",
				getpid(), getppid());
		printf("Reading a message from the parent (pid %d):\n", getppid());
		n = read(fd[0], line, 64);
		close(fd[1]);
		write(STDOUT_FILENO, line, n);
	}

	exit(0);
}
