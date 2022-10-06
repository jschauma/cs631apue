/* This program is all sorts of terrible.  If I catch
 * you doing anything like this in your code, you'll
 * have a hard time getting any points.
 *
 * We use this code to illustrate how "strings" work,
 * as well as how to inspect arrays and pointers using
 * gdb(1).
 *
 * Compile this with '-g'.  Run it as './a.out foo',
 * then run it in the debugger, break in 'main', and
 * inspect the different pointers at each step to
 * better understand what is going on.
 */
#include <stdio.h>
#include <string.h>

int
main(int argc, char **argv) {
	char *p = argv[0];
	char *q = argv[1];

	(void)argc;

	strcat(argv[0], "/");
	strcat(p, argv[1]);

	printf("argv[0]: %s\n", argv[0]);
	printf("argv[1]: %s\n\n", argv[1]);

	*(p + 1) = '\0';
	q[0] = 'm';

	printf("argv[0]: %s\n", argv[0]);
	printf("argv[1]: %s\n\n", argv[1]);

	p[1] = '/';

	printf("argv[0]: %s\n", argv[0]);

	return 0;
}
