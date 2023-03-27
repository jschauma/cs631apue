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

#define MALLOC_SIZE 32

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
	char *ptr;

	(void)printf("At program start:\n");
	printArray(envp, "envp");
	printArray(environ, "environ");

	(void)printf("Stack:\n");
	(void)printf("------\n");
	(void)printf("0x%12lX  int argc\n", (unsigned long)&argc);
	(void)printf("0x%12lX  char **argv\n", (unsigned long)&argv);
	(void)printf("0x%12lX  char **envp\n", (unsigned long)&envp);
	(void)printf("\n");

	(void)printf("Heap:\n");
	(void)printf("-----\n");
	if ((ptr = malloc(MALLOC_SIZE)) == NULL) {
		err(EXIT_FAILURE, "unable to allocate memory");
		/* NOTREACHED */
	}
	(void)printf("0x%12lX  malloced area ends\n", (unsigned long)ptr+MALLOC_SIZE);
	(void)printf("0x%12lX  malloced area begins\n", (unsigned long)ptr);
	free(ptr);
	(void)printf("\n");

	(void)printf("Uninitialized Data (BSS):\n");
	(void)printf("-------------------------\n");
	(void)printf("0x%12lX  extern char **environ\n", (unsigned long)&environ);
	(void)printf("\n");

	return EXIT_SUCCESS;
}
