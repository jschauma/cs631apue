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

/* This program illustrates the different behaviour between a system call
 * that is restarted after being interrupted and one where the call is
 * interrupted and we need to manually check this condition.
 *
 * This code example is derived from:
 * https://www.win.tue.nl/~aeb/linux/lk/lk-4.html
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int restarted = 0;

void
sig_handler(int signo) {
	(void)signo;
        restarted = 1;
}

int
main(void) {
	struct sigaction sa;
	int n;
	char c;

	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		(void)fprintf(stderr, "Unable to establish signal handler for SIGINT: %s\n",
			strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	/* Ensure interrupted system calls are restarted rather than
	 * setting errno to EINTR. */
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGQUIT, &sa, NULL) == -1) {
		(void)fprintf(stderr, "Unable to establish signal handler for SIGQUIT: %s\n",
			strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if ((n = read(STDIN_FILENO, &c, 1)) < 0) {
		if (errno == EINTR) {
			(void)printf("\nread call was interrupted\n");
		}
	} else if (restarted) {
		(void)printf("\nread call was restarted\n");
	}

	printf("|%c|\n", c);
	return EXIT_SUCCESS;
}
