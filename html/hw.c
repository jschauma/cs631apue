/* Use this program to illustrate that under pre-C99 C, you have to
 * provide a return code (compiler will warn), otherwise you get an
 * unpredictable return code (well, on some platforms; OS X, once again
 * differs).  Using '--std=c99', however, we default to a
 * return code of 0. */

#include <stdio.h>

int
main(int argc, char **argv) {
	printf("Hello World!\n");
}
