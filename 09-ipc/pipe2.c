/* This program illustrates a more common use of a pipe: one process
 * generates data, the other exec's another command with stdin connected
 * to the pipe's read end.  This is equivalent to
 *
 * cat file | ${PAGER:-/usr/bin/more}
 *
 */

#include <sys/wait.h>

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
		exit(1);
	}
	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "can't open %s\n", argv[1]);
		exit(1);
	}

	if (pipe(fd) < 0) {
		perror("pipe error");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	}
	else if (pid > 0) {		/* parent */
		close(fd[0]);		/* close read end */
		/* parent copies argv[1] to pipe */
		while (fgets(line, LINE_LENGTH, fp) != NULL) {
			n = strlen(line);
			if (write(fd[1], line, n) != n) {
				perror("write error to pipe");
				exit(1);
			}
		}
		if (ferror(fp)) {
			perror("fgets error");
			exit(1);
		}

		close(fd[1]);	/* close write end of pipe for reader */
		if (waitpid(pid, NULL, 0) < 0) {
			perror("waitpid error");
			exit(1);
		}
		exit(0);

	} else {		/* child */
		close(fd[1]);	/* close write end */
		if (fd[0] != STDIN_FILENO) {
			if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO) {
				perror("dup2 error to stdin");
				exit(1);
			}
			/* don't need this after dup2: */
			/* close(fd[0]); */
		}

		/* get arguments for execl() */
		if ((pager = getenv("PAGER")) == NULL)
			pager = DEF_PAGER;
		if ((argv0 = strrchr(pager, '/')) != NULL)
			argv0++;
		else
			argv0 = pager;

		/* We can of course lie about our name: */
		argv0 = "/bin/sh (not really)";
		execlp(pager, argv0, (char *) 0);
		fprintf(stderr,"execl error for %s\n", pager);
		exit(1);
	}

	return 0;
}
