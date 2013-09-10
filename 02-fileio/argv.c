/* A simple program to show what arguments were passed to a program.  This
 * can be used to illustrate how different shells expand arguments
 * differently.
 *
 * See also:
 * http://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html
 * http://content.hccfl.edu/pollock/ShScript/CmdLineProcessing.htm
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
