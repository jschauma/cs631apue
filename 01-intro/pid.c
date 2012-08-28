/*
 * This trivial program simply prints out its own PID and exits.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char **argv) {
	printf("Our process id is %d\n", getpid());
	return(0);
}
