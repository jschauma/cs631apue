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

/* This file is used to illustrate how to step through
 * a program at execution time using gdb(1).
 *
 * To this end, it contains a number of mistakes.
 *
 * See https://youtu.be/fOp4Q4mnTD4 for how to fix
 * them.
 */

/* The Fibonacci sequence is defined such that each number
 * is the sum of the two preceding ones. */

#include <stdio.h>
#include <stdlib.h>

int
fib(int i) {
	if (i == 0) {
		return 0;
	} else {
		return fib(i-2) + fib(i-1);
	}
}

int
main(int argc, char **argv) {
	if (argc != 2) {
		printf("usage: %s num\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	printf("%d\n", fib(atoi(argv[1])));
	exit(EXIT_SUCCESS);
}
