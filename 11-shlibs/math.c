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
	printf("%f rounded is %f\n", NUM, round(NUM));
	exit(EXIT_SUCCESS);
}
