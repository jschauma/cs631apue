/* This program allows us to visualize the layout of a
 * process in memory, printing the addresses of
 * various parts of the program, including components
 * in the text, data, and bss segments as well as on
 * the heap and the stack.
 *
 * This program can also illustrate a stack overflow
 * if compiled with '-DSTACKOVERFLOW'.
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

int
arraySize(char **a) {
	int n = 0;
	char **tmp = a;
	while (*tmp++) {
		n++;
	}
	return n;
}

void
printArray(char **a, char *name) {
	int n = arraySize(a);
	(void)printf("0x%12lX  %-7s[%d]\n", (unsigned long)&a[n], name, n);
	(void)printf("0x%12lX  %-7s[%d]    (0x%12lX '%s')\n", (unsigned long)&a[n-1], name, n-1, (unsigned long)a[n-1], a[n-1]);
	(void)printf("0x%12lX  %-7s[0]     (0x%12lX '%s')\n", (unsigned long)&a[0], name, (unsigned long)a[0], a[0]);
	(void)printf("\n");
}

int
main(int argc, char **argv, char **envp) {
	(void)argc;
	(void)argv;

	(void)printf("At program start:\n");
	printArray(envp, "envp");
	printArray(environ, "environ");

	if (setenv("FOO", "a longer value", 1) < 0) {
		err(EXIT_FAILURE, "unable to setenv(3)");
		/* NOTREACHED */
	}
	printf("After setenv(3):\n");
	printArray(envp, "envp");
	printArray(environ, "environ");

	if (putenv("BAR=another variable") < 0) {
		err(EXIT_FAILURE, "unable to putenv(3)");
		/* NOTREACHED */
	}
	printf("After putenv(3):\n");
	printArray(envp, "envp");
	printArray(environ, "environ");

	if (unsetenv("FOO") < 0) {
		err(EXIT_FAILURE, "unable to unsetenv(3)");
		/* NOTREACHED */
	}
	printf("After unsetenv(3):\n");
	printArray(envp, "envp");
	printArray(environ, "environ");

	return EXIT_SUCCESS;
}
