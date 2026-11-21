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

/* A simple program illustrating the implementation of a signal handler.
 * Note that we can use the same signal handler for multiple different
 * signals.
 */

#include <err.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void
sig_usr(int signo) {
	if (signo == SIGUSR1)
		(void)printf("Nobody expects SIGUSR1!\n");
	else if (signo == SIGUSR2)
		(void)printf("A surprising turn of events occurred!\n");
	else {
		(void)fprintf(stderr, "received signal: %d\n", signo);
		exit(EXIT_FAILURE);
	}
	return;
}

int
main(void) {
	if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
		err(EXIT_FAILURE, "unable to catch SIGUSR1");
		/* NOTREACHED */
	}
	if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
		err(EXIT_FAILURE, "unable to catch SIGUSR2");
		/* NOTREACHED */
	}

	if (signal(SIGHUP, sig_usr) == SIG_ERR) {
		err(EXIT_FAILURE, "unable to catch SIGHUP");
		/* NOTREACHED */
	}

	(void)printf("%d\n", getpid());
	for ( ; ; ) {
		pause();
		/* Note that the compiler is smart enough to realize we
		 * don't return from main, so won't warn that we didn't
		 * return a value. */
	}
}
