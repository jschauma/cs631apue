#include <stdio.h>
#include <unistd.h>

int
main(int argc, char **argv) {
	printf("Welcome to CS631 Advanced Programming in the UNIX Environment, %s!\n", getlogin());
}
