/*
 * show the difference between placements of the const keyword
 */

#include <stdio.h>

int main(int argc, char **argv) {

	/* A mutable pointer to a mutable string.  You
	 * can change the address the pointer points
	 * to as well as the contents of the location
	 * it points to. */
	char *foo = "oink";
	foo = "moo";
	foo[0] = 'w';

	/* A mutable pointer to an immutable string.
	 * You can change the address the pointer
	 * points to (1), but you cannot change the
	 * contents of the location it points to (2). */
	const char *bar = "oink";
	bar = "moo";		/* (1) This works. */
	bar[0] = 'w';		/* (2) This does not. */

	/* An immutable pointer to a mutable string.
	 * You cannot change what it points to (1), but
	 * you _can_ change the contents of what it
	 * points to (2). */
	char * const baz = "oink";
	baz = "moo";		/* (1) This doesn't work. */
	baz[0] = 'w';		/* (2) This does. */

	/* An immutable pointer to an immutable string.
	 * You cannot change anything about it. */
	const char * const qux = "oink";
	qux = "moo";		/* This will fail. */
	qux[0] = 'w';		/* As will this. */

	/* Now the same can be extended to arrays of
	 * strings. */

	/* This looks like a mutable pointer to
	 * mutable pointers.  We should be able to
	 * change anything about it. */

	char *wibble[] = { "oink", "moo", "woof", NULL };
	wibble[0] = "neigh";
	wibble[0][0] = 'w';

	/* An array looks and behaves a lot like a pointer,
	 * so we can have another pointer to this array of
	 * strings: */
	char **wobble = wibble;
	wobble[0] = "cluck";
	wobble[0][1] = 'w';

	/* But note that by declaring 'wibble' as an
	 * array, we are implicitly making it
	 * immutable: */
	char *wubble[] = { "honk", "snort", "screech", NULL };
	wibble = wubble;	/* This won't work. */

	/* So while 'char **' is fully mutable, an
	 * array like 'char *[]' really is a 'const
	 * char *[]': a (mutable) pointer to an
	 * immutable char array.  The values being
	 * pointed to (i.e. the elements in the array)
	 * can be changed, but not the pointer itself. */
	const char *blep[] = { "oink", "moo", "woof", NULL };
	blep = wubble;		/* This won't work. */
	blep[0] = "neigh";	/* This will work. */
	blep[0][0] = 'w';	/* This won't work. */


	/* Note: you can have a non-const pointer
	 * point to the const array above, but your
	 * compiler will warn you about that: */
	wobble = blep;		/* Warning only! */
	wobble[0] = "neigh";	/* This works! */
	wobble[0][0] = 'w';	/* As does this! */

	/* On the other hand, here we have an
	 * immutable pointer to a mutable array.  You
	 * can change the value of what each element
	 * in the array currently points to, but you
	 * cannot change the pointers themselves. */
	char * const blah[] = { "oink", "moo", "woof", NULL };
	blah = wubble;		/* This still won't work. */
	blah[0] = "nєigh";	/* This won't work. */
	blah[0][0] = 'w';	/* This will work. */

	/* And of course you can also have a
	 * completely immutable array of pointers: */
	const char * const boop[] = { "oink", "moo", "woof", NULL };
	boop = wubble;		/* This won't work. */
	boop[0] = "neigh";	/* Nor will this. */
	boop[0][0] = 'w';	/* Nor this. */

	return 1;
}
