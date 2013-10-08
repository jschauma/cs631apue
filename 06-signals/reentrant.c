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

	write(STDOUT_FILENO, "in signal handler\n", 18);
	if ((rootptr = getpwnam("root")) == NULL) {
		write(STDERR_FILENO, "no 'root' found!\n", 17);
		exit(1);
	}
	alarm(1);
}

int
main(void) {
	struct passwd *ptr;

	if (signal(SIGALRM, my_alarm) == SIG_ERR) {
		fprintf(stderr, "Unable to establish signal handler: %s\n", strerror(errno));
		exit(1);
	}

	alarm(1);

	for ( ; ; ) {
		if ((ptr = getpwnam(USER)) == NULL) {
			fprintf(stderr, "user %s not found!\n", USER);
			exit(1);
		}
		if(strcmp(ptr->pw_name, USER) != 0) {
			fprintf(stderr, "return value corrupted: pw_name = %s\n",
				ptr->pw_name);
			abort();
		}
	}
}
