/* A simple program illustrating the implementation of a signal handler.
 * Note that we can use the same signal handler for multiple different
 * signals.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void
sig_usr(int signo) {
	if (signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else {
		fprintf(stderr, "received signal: %d\n", signo);
		exit(1);
	}
	return;
}

int
main(void) {
	if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
		fprintf(stderr, "Can't catch SIGUSR1: %s", strerror(errno));
		exit(1);
	}
	if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
		fprintf(stderr, "Can't catch SIGUSR2: %s", strerror(errno));
		exit(1);
	}

	if (signal(SIGHUP, sig_usr) == SIG_ERR) {
		fprintf(stderr, "Can't catch SIGHUP: %s", strerror(errno));
		exit(1);
	}

	for ( ; ; )
		pause();
		/* Note that the compiler is smart enough to realize we
		 * don't return from main, so won't warn that we didn't
		 * return a value. */
}
