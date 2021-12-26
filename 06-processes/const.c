/*
 * Show the difference between placements of the const keyword.
 *
 * Note: you will get errors compiling this code.
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

int main() {

	/* You can do this, because 'one' is not 'const'... */
	foo[0][1] = 'w';

	/* ...but your compiler will complain here, because the
	 * 'foo' itself is 'const *'. */
	foo[0] = "blah blah blah";

	/* For 'bar', this is reversed: each element in the array
	 * is const, so we can't change them... */
	bar[0][1] = 'w';

	/* ...but because 'bar' is not const, we can change what
	 * this points to. */
	bar[0] = "blah blah blah";

	return 1;
}
