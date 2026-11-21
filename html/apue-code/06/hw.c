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

/* Use this program to illustrate that under pre-C99 C, you have to
 * provide a return code (compiler will warn), otherwise you get an
 * unpredictable return code (well, on some platforms; OS X, once again
 * differs).  Using '--std=c99', however, we default to a
 * return code of 0. */

#include <stdio.h>

int
main() {
	printf("hello, world!\n");
}
