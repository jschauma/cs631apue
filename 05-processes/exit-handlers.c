/* A simple illustration of exit handlers.  Note that exit handlers are
 * pushed onto a stack and thus execute in reverse order. */

#include <stdio.h>
#include <stdlib.h>

int i;

void
my_exit1(void) {
	printf("first exit handler: %d\n", i);
	i++;
}

void my_exit2(void) {
	printf("second exit handler: %d\n", i);
}


int
main(int argc, char **argv) {
	i = 0;
	if (atexit(my_exit2) != 0) {
		perror("can't register my_exit2\n");
		exit(1);
	}

	if (atexit(my_exit1) != 0) {
		perror("can't register my_exit1");
		exit(1);
	}

	if (atexit(my_exit1) != 0) {
		perror("can't register my_exit1");
		exit(1);
	}

	printf("main is done\n");

	return(0);
}
