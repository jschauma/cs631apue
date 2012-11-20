/* A very simple program to illustrate the different stages in the
 * compilation process.  First, use cpp(1) to pre-process the file; repeat
 * with FOOD defined to a different value.  Compare output, illustrate
 * that code is pulled in by the preprocessor and macros expanded.  Then,
 * compile hello.i to hello.s and show the resulting assembly output
 * before using as(1) to generate object code.  Use objdump(1) (or
 * similar) to disassemble the object code.  Finally, use ld(1) to create
 * an executable.
 *
 * The file 'dump' includes the traced output of the compiler invocation.
 * Show the different stages again.
 */
#include <stdio.h>

#ifndef FOOD
#define FOOD "Bacon"
#endif

int
main(int argc, char **argv) {
	printf("%s: great on anything.\n", FOOD);
	return 0;
}
