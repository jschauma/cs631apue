/*
 * show the difference between placements of the const keyword
 */


#include <stdio.h>

char * const foo[] = {
	"one",
	"two",
	"three",
	NULL
};

const char *bar[] = {
	"one",
	"two",
	"three",
	NULL
};

int main(int argc, char **argv) {

	foo[0][1] = 'w';
	foo[0] = "blah blah blah";

	bar[0][1] = 'w';
	bar[0] = "blah blah blah";

	return 1;
}
