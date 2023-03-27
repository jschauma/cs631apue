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
