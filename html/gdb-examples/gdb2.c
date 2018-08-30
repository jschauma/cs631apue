/* Another very simple program with a very obvious bug.  This time, run it
 * to create a core dump (remember to check 'ulimit -c' if necessary),
 * then use the debugger to analyze the core.  Run the executable after
 * setting a breakpoint and watching num and/or buf.
 */
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1 << 31

int num;

void
func2(char *buf) {
	fgets(buf, 1024, stdin);
}

void
func(void) {
	char *buf;

	num = BUFSIZE;
	buf = malloc(num);
	printf("What's new?\n");
	func2(buf);
	printf("Why do you say: '%s'?\n", buf);
}

int
main(int argc, char **argv) {
	num = 1024;
	func();
	return 0;
}
