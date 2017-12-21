/* A very simple program to illustrate the different stages in the
 * compilation process.  First, use cpp(1) to pre-process the file; repeat
 * with FOOD defined to a different value.  Compare output, illustrate
 * that code is pulled in by the preprocessor and macros expanded.  Then,
 * compile hello.i to hello.s and show the resulting assembly output
 * before using as(1) to generate object code.  Use objdump(1) (or
 * similar) to disassemble the object code.  Finally, use ld(1) to create
 * an executable.
 *
 * To illustrate compiler optimization, compile with
 * '-O0 -S' and once with '-O4 -S' and compare the
 * output.  You should notice that the calls to func1
 * and func2 are eliminated when optimization is
 * turned on.
 *
 * The file 'dump' includes the traced output of the compiler invocation.
 * Show the different stages again.
 */
#include <stdio.h>

#ifndef FOOD
#define FOOD "Bacon"
#endif

void
func2(void) {
	printf("%s: great on anything.\n", FOOD);
}

void
func1(void) {
	func2();
}

int
main(int argc, char **argv) {
	func1();
	return 0;
}
