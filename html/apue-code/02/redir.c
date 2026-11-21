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

/*
 * This simple program illustrates output redirection via dup(2).
 *
 * Compare:
 * ./a.out
 * ./a.out >/dev/null
 * ./a.out 2>/dev/null
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STDOUT_MSG "A message to stdout.\n"
#define STDERR_MSG "A message to stderr.\n"

void
writeBoth(const char *mark) {
	int len, marklen;

	marklen = strlen(mark);
	if (write(STDOUT_FILENO, mark, marklen) != marklen) {
		perror("unable to write marker to stdout");
		exit(EXIT_FAILURE);
	}

	len = strlen(STDOUT_MSG);
	if (write(STDOUT_FILENO, STDOUT_MSG, len) != len) {
		perror("unable to write to stdout");
		exit(EXIT_FAILURE);
	}

	if (write(STDERR_FILENO, STDERR_MSG, len) != len) {
		perror("unable to write to stdout");
		exit(EXIT_FAILURE);
	}
}

int
main() {
	writeBoth("before dup2\n");

	if (dup2(STDOUT_FILENO, STDERR_FILENO) < 0) {
		perror("Unable to redirect stderr to stdout");
		exit(EXIT_FAILURE);
	}

	writeBoth("after dup2\n");
}
