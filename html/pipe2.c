/* This program illustrates a more common use of a pipe: one process
 * generates data, the other exec's another command with stdin connected
 * to the pipe's read end.  This is equivalent to
 *
 * cat file | ${PAGER:-/usr/bin/more}
 *
 */

#include <sys/wait.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define	DEF_PAGER	"/usr/bin/more"
#define LINE_LENGTH	128			/* arbitrary! */

int
main(int argc, char **argv) {
	int n, fd[2];
	pid_t pid;
	char line[LINE_LENGTH], *pager, *argv0;
	FILE *fp;

	if (argc != 2) {
		perror("usage: a.out <pathname>");
		exit(EXIT_FAILURE);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "can't open %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (pipe(fd) < 0) {
		perror("pipe error");
		exit(EXIT_FAILURE);
	}

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0) {
		close(fd[0]);		/* close read end */
		while (fgets(line, LINE_LENGTH, fp) != NULL) {
			n = strlen(line);
			if (write(fd[1], line, n) != n) {
				perror("write error to pipe");
				exit(EXIT_FAILURE);
			}
		}
		if (ferror(fp)) {
			perror("fgets error");
			exit(EXIT_FAILURE);
		}

		/* We're done writing, so close write end of pipe. */
		close(fd[1]);
		if (waitpid(pid, NULL, 0) < 0) {
			perror("waitpid error");
			exit(EXIT_FAILURE);
		}
		exit(0);

	} else {
		close(fd[1]);	/* close write end */
		if (fd[0] != STDIN_FILENO) {
			if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO) {
				perror("dup2 error to stdin");
				exit(EXIT_FAILURE);
			}
		}

		/* get arguments for execl() */
		if ((pager = getenv("PAGER")) == NULL)
			pager = DEF_PAGER;
		if ((argv0 = strrchr(pager, '/')) != NULL)
			argv0++;
		else
			argv0 = pager;

		/* We can of course lie about our name: */
		argv0 = "tar";
		execlp(pager, argv0, (char *) 0);
		fprintf(stderr,"execl error for %s: %s\n", pager, strerror(errno));
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
