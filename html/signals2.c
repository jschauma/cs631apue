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

/* This program illustrates the previously default
 * behavior or signal handlers: each invocation reset
 * the disposition to SIG_DFL.
 *
 * Hit ^\ and we jump into sig_quit_reset, which
 * resets the signal disposition to the original
 * default (which is to quit with a core dump).  If
 * you hit ^\ while in sig_quit_reset, the signal is
 * queued and delivered as soon as the signal handler
 * terminates.  At that point, however, the default
 * signal handler has been installed, and the program
 * exits.
 *
 * This happens whether or not you exit the signal
 * handler normally or are transferred out of by
 * another signal.
 */

#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef SLEEP
#define SLEEP 5
#endif

int s = 0;

static void
sig_quit_reset(int signo) {
	if (signal(signo, SIG_DFL) == SIG_ERR) {
		err(EXIT_FAILURE, "unable to reset SIGQUIT");
		/* NOTREACHED */
	}

	(void)printf("In sig_quit_reset, s=%d. Now sleeping...\n", ++s);

	/* This call to sleep(3) can itself be
	 * interrupted if we receive a signal other
	 * than SIGQUIT while executing this signal
	 * handler.  If that happens, then we jump
	 * into the other signal handler; when that
	 * handler completes, we are returned back
	 * here. */
	(void)sleep(SLEEP);

	(void)printf("sig_quit_reset, s=%d: exiting\n", s);
}


static void
sig_int(int signo) {
	(void)signo;
	(void)printf("Now in sig_int, s=%d. Returning immediately.\n", ++s);
}

int
main(void) {
	(void)printf("\n=> Establishing initial signal hander via signal(3).\n");
	if (signal(SIGINT, sig_int) == SIG_ERR) {
		err(EXIT_FAILURE, "unable to set SIGINT signal handler");
		/* NOTREACHED */
	}

	(void)printf("\n=> Establishing a resetting signal hander via signal(3).\n");
	if (signal(SIGQUIT, sig_quit_reset) == SIG_ERR) {
		err(EXIT_FAILURE, "unable to set SIGQUIT signal handler");
		/* NOTREACHED */
	}
	(void)sleep(SLEEP);

	(void)printf("\n=> Time for a second interruption.\n");
	(void)sleep(SLEEP);

	(void)printf("Now exiting.\n");
	exit(EXIT_SUCCESS);
}
