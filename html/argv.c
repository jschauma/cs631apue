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

/* A simple program to show what arguments were passed to a program.  This
 * can be used to illustrate how different shells expand arguments
 * differently.
 *
 * See also:
 * http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html
 * https://wpollock.com/ShScript/CmdLineProcessing.htm
 *
 * Examples:
 * ./a.out
 * ./a.out *.c
 * ./a.out *.none
 * ./a.out *.[1c]
 * ./a.out "*.c"
 * ./a.out $USER
 * ./a.out "$(echo *.1)"
 * ./a.out {foo,bar,baz}.whatever
 * ./a.out {1..5}
 * ./a.out {1..5}{a..f}
 */

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv) {
	int i;

	for (i=0; i<argc; i++) {
		printf("%s\n", argv[i]);
	}

	return EXIT_SUCCESS;
}
