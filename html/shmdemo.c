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

/* A simple example to illustrate the use of shared
 * memory for IPC.
 *
 * Note that multiple invocations as a writer
 * overwrite the same location (unlike message
 * queues), and that shared memory, as all SysV IPC
 * structures, remain available after the program
 * exits and need to be cleaned up explicitly.
 *
 * Use ipcs(1) to show the use of the shared memory.
 */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Code examples often use a char as the ftok(3) id,
  * but really any int will do. */
#define SHM_ID     42
#define SHM_SIZE 1024

int
main(int argc, char **argv) {
	key_t key;
	int shmid;
	char *data;

	if (argc > 2) {
		(void)fprintf(stderr, "usage: shmdemo [data_to_write]\n");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if ((key = ftok("./shmdemo.c", SHM_ID)) == -1) {
		err(EXIT_FAILURE, "ftok");
		/* NOTREACHED */
	}

	if ((shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT)) == -1) {
		err(EXIT_FAILURE, "shmget");
		/* NOTREACHED */
	}

	data = shmat(shmid, (void *)0, 0);
	if (data == (char *)(-1)) {
		err(EXIT_FAILURE, "shmat");
		/* NOTREACHED */
	}

	if (argc == 2) {
		(void)printf("%d: writing to segment: \"%s\"\n", getpid(), argv[1]);
		(void)strncpy(data, argv[1], SHM_SIZE);
	} else
		(void)printf("%d: segment contains: \"%s\"\n", getpid(), data);

	if (shmdt(data) == -1) {
		err(EXIT_FAILURE, "shmdt");
		/* NOTREACHED */
	}

	return EXIT_SUCCESS;
}
