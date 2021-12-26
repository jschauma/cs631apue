/*
 * Version 2 of lecture 01's pid program, now with a
 * parent pid - woohoo!
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main() {
	(void)printf("My process id is %d.\n", getpid());
	(void)printf("My parent's process id is %d.\n", getppid());
	return EXIT_SUCCESS;
}
