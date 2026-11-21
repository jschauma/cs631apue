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

/* A simple program to illustrate that we can print
 * the address of any variable. */

#include <stdio.h>
#include <stdlib.h>

int
main() {
	int var;

	(void)printf("var is at: 0x%12lX\n", (unsigned long)&var);

	return EXIT_SUCCESS;
}
