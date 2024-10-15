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

#include <sys/shm.h>

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int
main(int argc, char **argv, char **envp) {
	int shmid;
	char *ptr;

	char func_array[ARRAY_SIZE];

	printf("Text Segment:\n");
	printf("-------------\n");
	printf("main (function) at                                     : 0x%12lX\n", (unsigned long)&main);
	printf("func (function) at                                     : 0x%12lX\n", (unsigned long)&func);
	printf("func2 (function) at                                    : 0x%12lX\n", (unsigned long)&func2);
	printf("\n");

	printf("Initialized Data:\n");
	printf("-----------------\n");
	printf("string (initialized global char *) at                  : 0x%12lX\n", (unsigned long)&string);
	printf("num (initialized global int) at                        : 0x%12lX\n", (unsigned long)&num);
	printf("\n");

	printf("Uninitialized Data (BSS):\n");
	printf("-------------------------\n");
	printf("extern **environ at                                    : 0x%12lX\n", (unsigned long)&environ);
	printf("string2 (uninitialized global char *) at               : 0x%12lX\n", (unsigned long)&string2);
	printf("num2 (uninitialized global int) at                     : 0x%12lX\n", (unsigned long)&num2);
	printf("array[] (uninitialized, fixed-size char * on BSS) from : 0x%12lX\n", (unsigned long)&array[0]);
	printf("array[] ends at                                        : 0x%12lX\n", (unsigned long)&array[ARRAY_SIZE]);
	printf("\n");

	printf("Heap:\n");
	printf("-----\n");
	if ((ptr = malloc(MALLOC_SIZE)) == NULL) {
		fprintf(stderr, "Unable to allocate memory: %s\n",
			strerror(errno));
		exit(1);
	}
	printf("malloced area begins at                                : 0x%12lX\n", (unsigned long)ptr);
	printf("malloced area ends at                                  : 0x%12lX\n", (unsigned long)ptr+MALLOC_SIZE);
	free(ptr);
	printf("\n");

	printf("Shared memory:\n");
	printf("--------------\n");

	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) {
		fprintf(stderr, "Unable to get shared memory: %s\n",
			strerror(errno));
		exit(1);
	}

	if ((ptr = shmat(shmid, 0, 0)) == (void *)-1) {
		fprintf(stderr, "Unable to map shared memory: %s\n",
			strerror(errno));
		exit(1);
	}
	printf("shared memory attachment begins at                     : 0x%12lX\n", (unsigned long)ptr);
	printf("shared memory attachment ends at                       : 0x%12lX\n", (unsigned long)ptr+SHM_SIZE);

	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		fprintf(stderr, "shmctl error: %s\n", strerror(errno));
		exit(1);
	}
	printf("\n");


	printf("High address (args and env):\n");
	printf("----------------------------\n");
	printf("argv at                                                : 0x%12lX\n", (unsigned long)&argv);
	printf("argc at                                                : 0x%12lX\n", (unsigned long)&argc);
	printf("last arg at                                            : 0x%12lX\n", (unsigned long)&argv[argc]);
	printf("environ[0] at                                          : 0x%12lX\n", (unsigned long)environ);
	printf("envp at                                                : 0x%12lX\n", (unsigned long)&envp);
	printf("envp[0] at                                             : 0x%12lX\n", (unsigned long)envp);
	printf("\n");

	if (setenv("FOO", "bar", 1) < 0) {
		fprintf(stderr, "Unable to setenv(3): %s\n", strerror(errno));
		exit(1);
	}
	printf("After setenv(3):\n");
	printf("environ[0] at                                          : 0x%12lX\n", (unsigned long)environ);
	printf("envp[0] at                                             : 0x%12lX\n", (unsigned long)envp);
	printf("\n");

	printf("Stack:\n");
	printf("------\n");
	printf("func_array[] (like 'array[]', but on stack) begins at  : 0x%12lX\n", (unsigned long)&func_array[0]);
	printf("func_array[] ends at                                   : 0x%12lX\n", (unsigned long)&func_array[ARRAY_SIZE]);
	printf("First variable inside main at                          : 0x%12lX\n", (unsigned long)&shmid);


	func(0);
	func2("from main");
	return 0;
}


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
	printf("static int n within func at                            : 0x%12lX\n", (unsigned long)&n);
	printf("func (called %d times): frame at                        : 0x%12lX\n", n, (unsigned long)&fint);
	n++;
	func2(msg);
}

void
func2(const char *how) {
	int fint;
	printf("func2 (%s): frame at                            : 0x%12lX\n", how, (unsigned long)&fint);
#ifdef STACKOVERFLOW
	func(1);
#endif
}
