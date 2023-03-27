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

/* This program illustrates that calls made from
 * within a signal handler may have an effect on the
 * world outside the signal handler.
 *
 * Specifically, using buffered I/O will interfere
 * with buffered I/O outside of the signal handler,
 * similar to what we've seen when we looked at how
 * fork(2) copies the output buffer.
 */

#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef SLEEP
#define SLEEP 5
#endif

#define MSG "SIGQUIT caught.\n"

static void
sig_quit(int signo) {
	(void)signo;
	//(void)printf(MSG);
	(void)write(STDOUT_FILENO, MSG, strlen(MSG));
}

int
main(int argc, char **argv) {
	(void)argv;

	if (signal(SIGQUIT, sig_quit) == SIG_ERR) {
		err(EXIT_FAILURE, "unable to set SIGQUIT signal handler");
		/* NOTREACHED */
	}

	(void)printf("=> Waiting for a signal...");
	if (argc > 1) {
		pause();
	}

	(void)printf(" done.\n");
	exit(EXIT_SUCCESS);
}
