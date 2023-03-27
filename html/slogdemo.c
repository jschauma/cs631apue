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

/* A simple program illustrating the use of syslog(3).
 * Just run the program, then send it SIGQUIT (^\),
 * or SIGINFO (^T), or SIGUSR[12].
 */

#include <err.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>

#define intlen(x) (int)(sizeof(x) / sizeof(int))
int SIGS[4] = { SIGINFO, SIGQUIT, SIGUSR1, SIGUSR2 };

static void
slog(int signo) {
	switch(signo) {
	/* in order of level */
	case SIGQUIT:
		syslog(LOG_ERR, "somebody sent us SIGQUIT");
		break;
	case SIGUSR1:
		/* FALLTHROUGH */
	case SIGUSR2:
		syslog(LOG_NOTICE, "SIGUSR");
		break;
	case SIGINFO:
		syslog(LOG_INFO, "received SIGINFO");
		break;
	default:
		err(EXIT_FAILURE, "Caught signal %d - this should never happen!", signo);
		/* NOTREACHED */
	}

	return;
}

int
main(void) {
	openlog("slogdemo", LOG_PERROR | LOG_PID, LOG_USER);

	for (int i = 0; i < intlen(SIGS); i++) {
		if (signal(SIGS[i], slog) == SIG_ERR) {
			err(EXIT_FAILURE, "unable to catch signal %d", i);
			/* NOTREACHED */
		}
	}

	(void)printf("%d\n", getpid());
	for ( ; ; ) {
		pause();
	}

	/* NOTREACHED */

	/* We never get here, but as an example of
	 * being a good citizen, we'd normally call
	 * closelog(3) just like we'd call close(2)
	 * for any file descriptor's we open(2)'d. */
	closelog();
}
