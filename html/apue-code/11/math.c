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
 * from the math header.  Linking the executable will fail,
 * unless we explicitly link against libm.
 *
 * Compare 'cc -Wall math.c -lm' with 'cc -Wall -rdynamic dlopenex.c
 * -dl'.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM 1.75

int
main(int argc, char **argv) {
	(void)argc;
	(void)argv;

	printf("%f rounded is %f\n", NUM, round(NUM));
	exit(EXIT_SUCCESS);
}
