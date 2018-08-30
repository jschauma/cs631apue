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
