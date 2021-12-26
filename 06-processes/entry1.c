#include <stdio.h>

int
main(int argc, char **argv) {
	printf("main is at 0x%lX\n", (unsigned long)&main);
}
