/* Another very simple program with a very obvious bug.  This time, run it
 * to create a core dump (remember to check 'ulimit -c' if necessary),
 * then use the debugger to analyze the core.  Run the executable after
 * setting a breakpoint and watching num and/or buf.
 */
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv) {
	char *buf;
	int num = 1 << 31;
	buf = malloc(num);
	fgets(buf, 1024, stdin);
	printf("%s\n", buf);
	return 1;
}
