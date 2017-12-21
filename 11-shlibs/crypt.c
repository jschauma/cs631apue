/* A simple program to introduce the concept of shared libraries.
 *
 * Compiling this program will work, as we have the forward declaration
 * from the header.  Linking the executable will fail,
 * unless we explicitly link against libcrypt.
 *
 * Compare
 *   cc -Wall crypt.c -lcrypt
 *   cc -Wall -rdynamic dlopenex.c -dl
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s string\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	printf("%s\n", crypt(argv[1], "$1"));
	exit(EXIT_SUCCESS);
}
