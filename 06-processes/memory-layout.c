/* A small program to print out the memory locations of the following:
 * - Block Started by Symgol (BSS) (via array[], string, string2)
 * - stack (via first variable in first function)
 * - variable on stack
 * - heap (via malloc)
 */

#include <sys/shm.h>

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARRAY_SIZE 40000
#define MALLOC_SIZE 100000

char array[ARRAY_SIZE];
char *string = "a string";
char *string2;
int num = 10;
int num2;

extern char **environ;
char **argv;
int argc;

void
func2(const char *how) {
	int fint;
	printf("func2 (%s): stack frame around %lX\n", how,
			(unsigned long)&fint);
#ifdef STACKOVERFLOW
	func2("recursive");
#endif
}

void
func() {
	int fint;
	printf("func: stack frame around %lX\n", (unsigned long)&fint);
	func2("from func");
}

int
main(int argc, char **argv) {
	int sint;
	char *ptr;

	char func_array[ARRAY_SIZE];

	printf("main (function) at %lX\n", (unsigned long)&main);
	printf("func (function) at %lX\n", (unsigned long)&func);
	printf("func2 (function) at %lX\n", (unsigned long)&func2);
	printf("environ at %lX\n", (unsigned long)&environ);
	printf("\n");

	printf("num (initialized global int) at %lX\n", (unsigned long)&num);
	printf("num2 (uninitialized global int) at %lX\n", (unsigned long)&num2);
	printf("string (initialized global char *) at %lX\n", (unsigned long)&string);
	printf("string2 (uninitialized global char *) at %lX\n", (unsigned long)&string2);

	printf("\n");

	printf("array[] (uninitialized, fixed-size char * on BSS) from %lX to %lX\n", (unsigned long)&array[0],
		(unsigned long)&array[ARRAY_SIZE]);
	printf("stack (first variable inside main) begins around %lX\n", (unsigned long)&sint);

	printf("func_array[] (like 'array[]', but on stack) from %lX to %lX\n",
		(unsigned long)&func_array[0],
		(unsigned long)&func_array[ARRAY_SIZE]);
	if ((ptr = malloc(MALLOC_SIZE)) == NULL) {
		fprintf(stderr, "Unable to allocate memory: %s\n",
			strerror(errno));
		exit(1);
	}
	printf("malloced from %lX to %lX\n", (unsigned long)ptr,
		(unsigned long)ptr+MALLOC_SIZE);
	free(ptr);

	printf("\n");

	func();
	printf("func popped off\n");
	func2("from main");

	return 0;
}
