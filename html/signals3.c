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

/* This program illustrates blocking a signal and the
 * use of signal masks.
 *
 * SIG_QUIT is blocked, so if we hit ^\, nothing
 * happens.  We can check whether any such signals
 * were delivered via sigpending(2). Once we unblock
 * the signal, we will immediately enter the signal
 * handler.
 *
 * Note that if we change the signal handler to
 * SIG_IGN _after_ the signal has been generated, but
 * _before_ we unblock, it will still be ignored.
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
main(int argc, char **argv) {
	sigset_t newmask, oldmask, pendmask;
	int ismember = 0;

	(void)argv;
	(void)printf("\n=> Establishing initial signal hander via signal(3).\n");
	if (signal(SIGQUIT, sig_quit) == SIG_ERR) {
		err(EXIT_FAILURE, "unable to set SIGQUIT signal handler");
		/* NOTREACHED */
	}

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		err(EXIT_FAILURE, "unable to set SIGINT signal handler");
		/* NOTREACHED */
	}

	if (sigemptyset(&newmask) < 0) {
		err(EXIT_FAILURE, "sigemtpyset");
		/* NOTREACHED */
	}
	if (sigaddset(&newmask, SIGQUIT) < 0) {
		err(EXIT_FAILURE, "sigaddset");
		/* NOTREACHED */
	}

	(void)printf("\n=> Blocking delivery of SIGQUIT...\n");
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		err(EXIT_FAILURE, "unable to block SIGQUIT");
		/* NOTREACHED */
	}

	(void)printf("\n=> Now going to sleep for %d seconds...\n", SLEEP);
	(void)sleep(SLEEP);

	if (argc > 1) {
		if (signal(SIGQUIT, SIG_IGN) == SIG_ERR) {
			err(EXIT_FAILURE, "unable to ignore SIGQUIT");
			/* NOTREACHED */
		}
	}

	printf("\n=> Checking if any signals are pending...\n");
	if (sigpending(&pendmask) < 0) {
		err(EXIT_FAILURE, "sigpending");
		/* NOTREACHED */
	}

	if ((ismember = sigismember(&pendmask, SIGQUIT)) < 0) {
		err(EXIT_FAILURE, "sigismember");
		/* NOTREACHED */
	}

	if (ismember) {
		(void)printf("Pending SIGQUIT found.\n");
	}

	(void)printf("\n=> Unblocking SIGQUIT...\n");
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		err(EXIT_FAILURE, "unable to unblock SIGQUIT");
		/* NOTREACHED */
	}

	/* Note that if we did receive ^\ while we
	 * were blocked, we are jumping into sig_quit
	 * right here, _before_ we print this. */
	(void)printf("SIGQUIT unblocked - sleeping some more...\n");

	/* ^\ will now be delivered again */
	(void)sleep(SLEEP);

	(void)printf("Now exiting.\n");
	exit(EXIT_SUCCESS);
}
