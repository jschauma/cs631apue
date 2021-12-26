#include <stdio.h>
#include <stdlib.h>

int
foo(void) {
	(void)printf("Who needs 'main'?\n");
	return EXIT_FAILURE;
}

int
main(int argc, char **argv) {
	printf("main is at 0x%lX\n", (unsigned long)&main);
}
