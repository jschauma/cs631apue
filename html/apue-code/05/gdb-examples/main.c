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

/* This file is used together with the file 'buf.c' to
 * help students use gdb(1) to understand
 * manipulations of strings and buffers.
 *
 * See https://youtu.be/hgcj7iAxhhU as well as
 * 'pointer.c'
 */

#include <stdlib.h>
#include <stdio.h>

void printBufs(long);

int
main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s num\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	printBufs(strtol(argv[1], NULL, 10));
}
