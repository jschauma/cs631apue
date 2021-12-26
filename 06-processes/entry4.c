#include <stdio.h>

int
main() {
	int n;
	n = printf("main is at 0x%lX\n", (unsigned long)&main);
	return n;
}
