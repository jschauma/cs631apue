/* A simple program to help illustrate the use of dlopen(3) in dlopenex.c:
 *
 * Compiling this program will work, as we have the forward declaration
 * from the bsd compatibility header.  Linking the executable will fail,
 * unless we explicitly link against libbsd.
 *
 * Compare 'cc -Wall setget.c -lbsd' with 'cc -Wall -rdynamic dlopenex.c
 * -dl'.
 */

#include <bsd/stdlib.h>
#include <stdio.h>

int
main(int argc, char **argv) {
	setprogname("setget");
	printf("My name is '%s'.\n", getprogname());
	exit(EXIT_SUCCESS);
}
