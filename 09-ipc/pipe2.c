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

/* This program illustrates a more common use of a
 * pipe: one process generates data, the other exec's
 * another command with stdin connected to the pipe's
 * read end.  This is equivalent to
 *
 * cat file | ${PAGER:-/usr/bin/more}
 *
 */

#include <sys/wait.h>

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define	DEF_PAGER	"/usr/bin/more"
#define LINE_LENGTH	128			/* arbitrary! */

int
main(int argc, char **argv)
{
	int n, fd[2];
	pid_t pid;
	char line[LINE_LENGTH], *pager, *argv0;
	FILE *fp;

	if (argc != 2) {
		(void)fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		(void)fprintf(stderr, "unable to open %s: %s\n",
				argv[1], strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if (pipe(fd) < 0) {
		err(EXIT_FAILURE, "pipe");
		/* NOTREACHED */
	}

	if ((pid = fork()) < 0) {
		err(EXIT_FAILURE, "fork");
		/* NOTREACHED */
	}

	if (pid > 0) {
		(void)close(fd[0]);		/* close read end */
		while (fgets(line, LINE_LENGTH, fp) != NULL) {
			n = strlen(line);
			if (write(fd[1], line, n) != n) {
				err(EXIT_FAILURE, "write to pipe");
				/* NOTREACHED */
			}
		}
		if (ferror(fp)) {
			err(EXIT_FAILURE, "fgets");
			/* NOTREACHED */
		}

		/* We're done writing, so close write end of pipe. */
		(void)close(fd[1]);
		if (waitpid(pid, NULL, 0) < 0) {
			err(EXIT_FAILURE, "waitpid");
			/* NOTREACHED */
		}
		exit(EXIT_SUCCESS);
		/* NOTREACHED */
	} else {
		(void)close(fd[1]);	/* close write end */
		if (fd[0] != STDIN_FILENO) {
			if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO) {
				err(EXIT_FAILURE, "dup2 to stdin");
				/* NOTREACHED */
			}
		}

		/* get arguments for execl() */
		if ((pager = getenv("PAGER")) == NULL)
			pager = DEF_PAGER;
		if ((argv0 = strrchr(pager, '/')) != NULL)
			argv0++;
		else
			argv0 = pager;

		/* We could of course lie about our name: */
		argv0 = "tar";
		execlp(pager, argv0, (char *) 0);
		(void)fprintf(stderr, "execl error for %s: %s\n",
				pager, strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	/* NOTREACHED */
}
