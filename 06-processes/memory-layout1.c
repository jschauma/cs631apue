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
