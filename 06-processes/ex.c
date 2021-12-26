#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
func2() {
	_exit(0xcafe);
}

int
func() {
	exit(func2());
}

int
main() {
	printf("%d\n", func());
}
