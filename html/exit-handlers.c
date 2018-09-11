/* A simple illustration of exit handlers.  Note that exit handlers are
 * pushed onto a stack and thus execute in reverse order. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int i;

void
my_exit1(void) {
	printf("first exit handler: %d\n", i);
	i++;
}

void
my_exit2(void) {
	printf("second exit handler: %d\n", i);
}

void
func(int argc) {
	printf("In func.\n");
	if (argc == 2) {
		exit(0);
	} else if (argc == 3) {
		_exit(0);
	}
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

	func(argc);

	printf("main is done\n");

	return(0);
}
