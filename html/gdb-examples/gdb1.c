/* A very simple program with a very obvious error.  Use this to show how
 * to set a breakpoint and step through code execution.
 */
#include <ctype.h>
#include <stdio.h>

int
main(int argc, char **argv) {
	char c;
	c = fgetc(stdin);
	while (c != EOF)
		if(isalnum(c))
			printf("%c\n", c);
		c = fgetc(stdin);

	return 1;
}
