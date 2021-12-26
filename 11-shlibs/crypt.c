/* A simple program to introduce the concept of shared libraries.
 *
 * Compiling this program will work, as we have the forward declaration
 * from the header.  Linking the executable will fail,
 * unless we explicitly link against libcrypt.
 *
 * Compare
 *   cc -Wall -Werror -Wextra crypt.c -lcrypt
 *   cc -Wall -Werror -Wextra -rdynamic dlopenex.c -dl
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void
printCrypt(const char *s) {
	(void)printf("%s\n", crypt(s, "$1"));
}

int
main(int argc, char **argv) {
	if (argc != 2) {
		(void)fprintf(stderr, "Usage: %s string\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	printCrypt(argv[1]);
	exit(EXIT_SUCCESS);
}
