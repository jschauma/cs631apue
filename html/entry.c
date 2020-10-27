/* This program can be used to illustrates how the kernel sets up the
 * startup routine.  Call with "-e" to specify any of the functions as the
 * entry point instead of 'main'; compare otool(1)/objdump(1) output and
 * exit codes.
 *
 * To display the entry point of the program:
 * NetBSD:
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

int
bar(void) {
	printf("bar rules!\n");
	exit(EXIT_FAILURE);
	/* Unlike foo(), this will not cause a segfault, since we are not
	 * returning; we explicitly call exit(3). */
}

int
foo(void) {
	printf("Foo for the win!\n");
	return EXIT_FAILURE;
	/* Note: this will cause a segfault on NetBSD, because this function
	 * returns, but there is nothing to return to: the routines set up
	 * by the kernel remain set up for 'main', we just told the linker
	 * to jump into 'foo' at program start.  Compare objdump(1)
	 * output.
	 * Note: on OS X, we do not segfault! */
}

int
main(int argc, char **argv) {
	printf("main is at 0x%lX\n", (unsigned long)&main);
	/* Note that we do explicitly _not_ return an error here, nor call
	 * any of the exit(3) functions.  Your compiler may warn you
	 * about this. We're also not casting the return value of printf(3)
	 * to void. Inspect the return value. */
}
