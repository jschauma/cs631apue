#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLUS "plus"
#define TIMES "times"

int func2(int a, int b) {
	return a*b;
}

int func1(int a, int b) {
	return a+b;
}

int (*selectFunc(const char *s))(int, int) {
	if (strncmp(s, PLUS, strlen(PLUS)) == 0) {
		return func1;
	} else if (strncmp(s, TIMES, strlen(TIMES)) == 0) {
		return func2;
	} else {
		fprintf(stderr, "Invalid argument '%s'.\n", s);
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char **argv) {
	int (*func)(int, int);

	setprogname("fp");

	if (argc != 4) {
		fprintf(stderr, "Usage: %s <plus|times> num1 num2\n", getprogname());
		return EXIT_FAILURE;
	}

	func = selectFunc(argv[1]);
	printf("%d\n", func(atoi(argv[2]), atoi(argv[3])));
	return EXIT_SUCCESS;
}
