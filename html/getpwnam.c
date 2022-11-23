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
		fprintf(stderr, "no 'root' found!\n");
		exit(1);
	}
	alarm(1);
}

int main() {
	struct passwd *ptr;

	signal(SIGALRM, my_alarm);
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
