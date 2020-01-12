/* A small program to print out the memory locations
 * of various things on the program, including:
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
#define SHM_SIZE 100000
#define SHM_MODE 0600

char array[ARRAY_SIZE];
char *string = "a string";
char *string2;
int num = 10;
int num2;

extern char **environ;
char **argv;
int argc;

void func(int);
void func2(const char *);

void
func(int i) {
	int fint;
	/* Change this value to 0 and note how
	 * the location of where it is stored
	 * changes from the Data to BSS segment. */
	static int n = 1;
	char *msg = "from func";
	if (i) {
		msg = "recursive";
	}
	printf("n (%d) at %lX\n", n, (unsigned long)&n);
	printf("func (called %d times): stack frame around %lX\n", n, (unsigned long)&fint);
	n++;
	func2(msg);
}

void
func2(const char *how) {
	int fint;
	printf("func2 (%s): stack frame around %lX\n", how,
			(unsigned long)&fint);
#ifdef STACKOVERFLOW
	func(1);
#endif
}

int
main(int argc, char **argv, char **envp) {
	int sint;
	int shmid;
	char *ptr, *shmptr;

	char func_array[ARRAY_SIZE];

	printf("main (function) at %lX\n", (unsigned long)&main);
	printf("argc at %lX\n", (unsigned long)&argc);
	printf("argv at %lX\n", (unsigned long)&argv);
	printf("last arg at %lX\n", (unsigned long)&argv[argc]);
	printf("func (function) at %lX\n", (unsigned long)&func);
	printf("func2 (function) at %lX\n", (unsigned long)&func2);
	printf("environ at %lX\n", (unsigned long)&environ);
	printf("environ[0] at to %lX\n", (unsigned long)environ);
	printf("envp at %lX\n", (unsigned long)&envp);
	printf("envp[0] at %lX\n", (unsigned long)envp);
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

	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) {
		fprintf(stderr, "Unable to get shared memory: %s\n",
				strerror(errno));
		exit(1);
	}

	if ((shmptr = shmat(shmid, 0, 0)) == (void *)-1) {
		fprintf(stderr, "Unable to map shared memory: %s\n",
				strerror(errno));
		exit(1);
	}
	printf("shared memory attached from %lx to %lx\n", (unsigned long)shmptr,
			(unsigned long)shmptr+SHM_SIZE);

	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		fprintf(stderr, "shmctl error: %s\n", strerror(errno));
		exit(1);
	}

	printf("\n");

	func(0);
	printf("func popped off\n");
	func2("from main");

	return 0;
}
