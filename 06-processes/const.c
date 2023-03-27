/* This file is part of the sample code and exercises
 * used by the class "Advanced Programming in the UNIX
 * Environment" taught by Jan Schaumann
 * <jschauma@netmeister.org> at Stevens Institute of
 * Technology.
 *
 * This file is in the public domain.
 *
 * You don't have to, but if you feel like
 * acknowledging where you got this code, you may
 * reference me by name, email address, or point
 * people to the course website:
 * https://stevens.netmeister.org/631/
 */

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
