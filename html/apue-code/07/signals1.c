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

/* This program illustrates that you can be
 * interrupted while executing a signal handler.
 *
 * It also shows that the signal for the currently
 * executing handler is blocked until we return from
 * that call, but that pending signals are merged and
 * then delivered afterwards.
 *
 * To demonstrate:
 * Hit ^\ to jump into sig_quit.

 * Hit ^\ again and note that we're not interrupting
 * the sleep inside the sig_quit handler.  However,
 * the signal gets delivered once sig_quit finishes,
 * so we re-enter sig_quit.
 *
 * Multiple signals of the same kind are merged, so
 * hitting ^\ multiple times while in sig_quit only
 * yields a single signal being delivered after we
 * finish in sig_quit.
 *
 * However: hit ^\, then ^C and note that sig_int
 * executes immediately.  We were transferred out of
 * sig_quit, then returned immediately back into
 * sig_quit.
 *
 * Finally, note that if you hit ^\, then hit ^\
 * again, then ^C, you should see us entering
 * sig_quit, then sig_int, then re-enter sig_quit
 * without the first sig_quit invocation having
 * terminated, since jumping out of the handler
 * unblocked the signal.
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

int s = 0;

static void
sig_quit(int signo) {
	(void)signo;
	(void)printf("In sig_quit, s=%d. Now sleeping...\n", ++s);

	/* This call to sleep(3) can itself be
	 * interrupted if we receive a signal other
	 * than SIGQUIT while executing this signal
	 * handler.  If that happens, then we jump
	 * into the other signal handler; when that
	 * handler completes, we are returned back
	 * here. */
	(void)sleep(SLEEP);

	(void)printf("sig_quit, s=%d: exiting\n", s);
}

static void
sig_int(int signo) {
	(void)signo;
	(void)printf("Now in sig_int, s=%d. Returning immediately.\n", ++s);
}

int
main(void) {
	(void)printf("\n=> Establishing initial signal hander via signal(3).\n");
	if (signal(SIGQUIT, sig_quit) == SIG_ERR) {
		err(EXIT_FAILURE, "unable to set SIGQUIT signal handler");
		/* NOTREACHED */
	}

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		err(EXIT_FAILURE, "unable to set SIGINT signal handler");
		/* NOTREACHED */
	}

	(void)sleep(SLEEP);

	(void)printf("\n=> Time for a second interruption.\n");
	(void)sleep(SLEEP);

	(void)printf("Now exiting.\n");
	exit(EXIT_SUCCESS);
}
