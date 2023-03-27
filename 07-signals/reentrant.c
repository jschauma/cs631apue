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

/* This program illustrates the risk of calling non-re-entrant functions
 * in your signal handler.  Depending on the platform, the results of
 * running this program may differ, but you will see a number of possible
 * results:
 * - user not found: you accidentally 'skipped' over the entry you were
 *                   looking for
 * - return value corrupted: you got back a different struct passwd then
 *                           the one you requested
 * - memory fault: FUBAR
 */

#include <err.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef USER
#define USER "jschauma"
#endif

static void
my_alarm(int signo)
{
	struct passwd *rootptr;

	(void)signo;
	(void)write(STDOUT_FILENO, "SIGALRM\n", 8);
	for (int i=0; i<3; i++) {
		if ((rootptr = getpwnam("root")) == NULL) {
			(void)write(STDERR_FILENO, "no 'root' found!\n", 17);
			exit(EXIT_FAILURE);
			/* NOTREACHED */
		}
	}
	if (alarm(1) == (unsigned int)-1) {
		err(EXIT_FAILURE, "alarm");
		/* NOTREACHED */
	}
}

int
main(void) {
	struct passwd *ptr;

	if (signal(SIGALRM, my_alarm) == SIG_ERR) {
		fprintf(stderr, "Unable to establish signal handler: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if (alarm(1) == (unsigned int)-1) {
		err(EXIT_FAILURE, "alarm");
		/* NOTREACHED */
	}

	for ( ; ; ) {
		if ((ptr = getpwnam(USER)) == NULL) {
			fprintf(stderr, "user %s not found!\n", USER);
			exit(EXIT_FAILURE);
			/* NOTREACHED */
		}
		if (strcmp(ptr->pw_name, USER) != 0) {
			fprintf(stderr, "return value corrupted: pw_name = %s\n",
				ptr->pw_name);
			abort();
			/* NOTREACHED */
		}
	}
}
