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

/* This program creates as many worker children as
 * requested.  To facilitate identification in the
 * process table, we self-exec with a fake argv[0].
 */

#include <sys/wait.h>

#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#define ARGV0_LENGTH 8 + 1 /* "worker" + 2 digits + NULL */
#define MAXCHILDREN 32

void
work() {
	int i = 0;
	while (1) {
		i++;
	}
}

int
main(int argc, char **argv) {
	int n;
	pid_t pid;

	if (argc == 1) {
		work();
		/* NOTREACHED */
	} else if (argc > 2) {
		(void)fprintf(stderr, "Usage: %s num\n", argv[0]);
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	n = atoi(argv[1]);
	if ((n < 1) || (n > MAXCHILDREN)) {
		(void)fprintf(stderr, "Please provide a number > 0 and <= %d.\n", MAXCHILDREN);
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	for (int i = 0; i < n; i++) {
		if ((pid = fork()) < 0) {
			err(EXIT_FAILURE, "fork");
			/* NOTREACHED */
		} else if (!pid) {
			char argv0[ARGV0_LENGTH];
			bzero(argv0, ARGV0_LENGTH);

			(void)snprintf(argv0, ARGV0_LENGTH, "worker%d", i);

			(void)printf("Created %s: %d\n", argv0, getpid());

			char * const args[] = { argv0, NULL };
			execve("./a.out", args, NULL);
			err(EXIT_FAILURE, "execve");
			/* NOTREACHED */
		}
	}

	while ((pid = wait(NULL)) > 0);
	if ((pid < 0) && errno != ECHILD) {
		err(EXIT_FAILURE, "wait");
		/* NOTREACHED */
	}

	return EXIT_SUCCESS;
}
