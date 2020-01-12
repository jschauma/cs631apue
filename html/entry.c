/* This program can be used to illustrates how the kernel sets up the
 * startup routine.  Call with "-e" to specify any of the functions as the
 * entry point instead of 'main'; compare otool(1)/objdump(1) output and
 * exit codes.
 *
 * To display the entry point of the program:
 * Linux / NetBSD:
 * - readelf -h a.out | grep ntry
 * - objdump -d a.out
 *
 * Mac OS X:
 * - otool -l a.out | grep ntry # decimal address
 * - otool -v -t a.out          # hex address
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAINMSG "Hooray Main!\n"

int foo(void) {
	printf("Foo for the win!\n");
	return 1;
	/* Note: this will cause a segfault on Linux, because this function
	 * returns, but there is nothing to return to: the routines set up
	 * by the kernel remain set up for 'main', we just told the linker
	 * to jump into 'foo' at program start.  Compare objdump(1)
	 * output.
	 * Note: on OS X, we do not segfault! */
}

int bar(int argc, char **argv) {
	printf("%s rules!\n", argv[0]);
	exit(1);
	/* Unlike foo(), this will not cause a segfault, since we are not
	 * returning; we explicitly call exit(3). */
}

int main(int argc, char **argv) {
	printf(MAINMSG);
	/* Note that we do explicitly _not_ return an error here, nor call
	 * any of the exit(3) functions.  Your compiler will warn you
	 * about this. Inspect the return value. */
}
