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

/* A small program to print out the memory locations of the following:
 * - BSS (via array[])
 * - stack (via first variable in first function)
 * - variable on stack
 * - heap (via malloc)
 * - shared memory (vi shmat)
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

int
main(int argc, char **argv) {
	(void)argc;
	(void)argv;

	int shmid;
	char *ptr, *shmptr;

	char func_array[ARRAY_SIZE];

	printf("array[] from %lx to %lx\n", (unsigned long)&array[0],
		(unsigned long)&array[ARRAY_SIZE]);
	printf("stack around %lx\n", (unsigned long)&shmid);

	printf("func_array[] (on stack) from %lx to %lx\n",
		(unsigned long)&func_array[0],
		(unsigned long)&func_array[ARRAY_SIZE]);
	if ((ptr = malloc(MALLOC_SIZE)) == NULL) {
		fprintf(stderr, "Unable to allocate memory: %s\n",
			strerror(errno));
		exit(1);
	}
	printf("malloced from %lx to %lx\n", (unsigned long)ptr,
		(unsigned long)ptr+MALLOC_SIZE);

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

	return 0;
}
