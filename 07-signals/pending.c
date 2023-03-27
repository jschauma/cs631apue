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

/* This program illustrates a number of perhaps confusing features of
 * signal delivery.  It shows how signals can be blocked, how pending
 * signals are merged, how you can jump out of a signal handler into
 * another, how resetting signal handlers differ from non-resetting signal
 * handlers  and how to check whether or not pending signals are of a
 * certain type.
 *
 * You want to run this program a number of times to illustrate all of the
 * different features.  Hopefully the commentary in the code below will
 * help you remember all the things you want to demonstrate.
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef SLEEP
#define SLEEP 5
#endif

int s = 0;

void
sig_quit_reset(int signo) {
	fprintf(stderr, "sig_quit_reset: caught SIGQUIT (%d), sleeping and resetting.\n", ++s);
	sleep(SLEEP);
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR) {
		fprintf(stderr, "can't reset SIGQUIT\n");
		exit(1);
	}
	fprintf(stderr, "sig_quit_reset: restored SIGQUIT handler to default.\n");
}


void
sig_quit(int signo) {
	fprintf(stderr, "sig_quit: caught SIGQUIT (%d), now sleeping\n", ++s);
	sleep(SLEEP);
	fprintf(stderr, "sig_quit: exiting (%d)\n", s);
}

void
sig_int(int signo) {
	fprintf(stderr, "sig_int: caught SIGINT (%d), returning immediately\n", ++s);
}

int main() {
	sigset_t newmask, oldmask, pendmask;
	int ismember = 0;

	printf("\n=> Establishing initial signal hander via signal(3).\n");
	if(signal(SIGQUIT, sig_quit) == SIG_ERR) {
		fprintf(stderr, "can't set signal handler\n");
		exit(1);
	}

	if(signal(SIGINT, sig_int) == SIG_ERR) {
		fprintf(stderr, "can't set signal handler\n");
		exit(1);
	}

	/* Hit ^\ to jump into sig_quit.
	 * Hit ^\ again and note that we're not interrupting the sleep
	 * inside the sig_quit handler.  However, the signal gets
	 * delivered once sig_quit finishes, so we re-enter sig_quit.
	 *
	 * Multiple signals of the same kind are merged, so hitting ^\
	 * multiple times while in sig_quit only yields a single signal
	 * being delivered after we finish in sig_quit.
	 *
	 * However: hit ^\, then ^C and note that sig_int executes
	 * immediately.  We were transferred out of sig_quit, then
	 * returned immediately back into sig_quit.
	 *
	 * If we start with one ^\ followed by ^C, then we immediately
	 * move on establishing the resetting signal handler; if we hit ^\
	 * multiple times followed by ^C, then our first ^\ is caught, the
	 * subsequent ones are merged and queued, sig_int is executed and
	 * the queued ^\ delivered.
	 */

	sleep(SLEEP);

	printf("\n=> Time for a second interruption.\n");
	sleep(SLEEP);

	printf("\n=> Establishing a resetting signal hander via signal(3).\n");
	if(signal(SIGQUIT, sig_quit_reset) == SIG_ERR) {
		fprintf(stderr, "can't set signal handler\n");
		exit(1);
	}

	/* If we repeat the same thing here, things are rather different:
	 * Hit ^\ and we jump into sig_quit_reset, which resets the signal
	 * disposition to the original default (which is to quit with a
	 * core dump).  If you hit ^\ while in sig_quit_reset, the signal
	 * is queued and delivered as soon as the signal handler
	 * terminates.  At that point, however, the default signal handler
	 * has been installed, and the program exits.
	 *
	 * If we do not deliver a second ^\, then we establish a blocking
	 * mask for this signal, allowing multiple subsequent ^\ to be
	 * blocked.  We then report on whether or not we have any, and
	 * finally unblock them, ready to be delivered (to the default
	 * handler).
	 */
	sleep(SLEEP);

	printf("\n=> Time for a second interruption.\n");
	sleep(SLEEP);

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);

	printf("\n=> Blocking delivery of SIGQUIT...\n");
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		fprintf(stderr, "SIG_BLOCK error\n");
		exit(1);
	}

	printf("\n=> Now going to sleep for %d seconds...\n", SLEEP);
	sleep(SLEEP);

	printf("\n=> Checking if any signals are pending...\n");
	if (sigpending(&pendmask) < 0) {
		fprintf(stderr, "sigpending error\n");
		exit(1);
	}

	ismember = sigismember(&pendmask, SIGQUIT);
	if (ismember < 0) {
		fprintf(stderr, "sigismember error\n");
		exit(1);
	} else if (ismember == 1) {
		printf("Pending SIGQUIT found.\n");
	}

	printf("\n=> Unblocking SIGQUIT...\n");
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		fprintf(stderr, "SIG_SETMASK error\n");
		exit(1);
	}

	printf("SIGQUIT unblocked - sleeping some more...\n");

	/* Finally, if we have not delivered a second ^\ after the
	 * resetting handler, then we get here and can simply deliver
	 * another ^\ now, exiting the program.
	 */

	sleep(SLEEP);

	printf("Now exiting.\n");
	exit(0);
}
