#include <stdio.h>
#include <unistd.h>

int
main() {
	printf("Welcome to CS631 Advanced Programming in the UNIX Environment, %s!\n", getlogin());
	return 0;
}
