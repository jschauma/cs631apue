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

/* A simple program to begin our exploration of the
 * layout of a process in memory. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global;

int
main(int argc, char **argv) {
	int private;

	(void)printf("argc at   : 0x%12lX\n", (unsigned long)&argc);
	(void)printf("argv at   : 0x%12lX\n", (unsigned long)&argv);
	(void)printf("private at: 0x%12lX\n", (unsigned long)&private);
	(void)printf("global at : 0x%12lX\n", (unsigned long)&global);
	(void)printf("main at   : 0x%12lX\n", (unsigned long)&main);

	return EXIT_SUCCESS;
}
