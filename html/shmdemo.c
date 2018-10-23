/* A simple example to illustrate the use of shared memory for IPC.
 *
 * Note that multiple invocations as a writer overwrite the same location
 * (unlike message queues), and that shared memory, as all SysV IPC
 * structures, remain available after the program exits and need to be
 * cleaned up explicitly.
 *
 * Use ipcs(1) to show the use of the shared memory.
 */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHM_SIZE 1024

int
main(int argc, char **argv) {
	key_t key;
	int shmid;
	char *data;

	if (argc > 2) {
		fprintf(stderr, "usage: shmdemo [data_to_write]\n");
		exit(1);
	}

	if ((key = ftok("shmdemo.c", 'R')) == -1) {
		perror("ftok");
		exit(1);
	}

	if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
		perror("shmget");
		exit(1);
	}

	data = shmat(shmid, (void *)0, 0);
	if (data == (char *)(-1)) {
		perror("shmat");
		exit(1);
	}

	if (argc == 2) {
		printf("writing to segment: \"%s\"\n", argv[1]);
		strncpy(data, argv[1], SHM_SIZE);
	} else
		printf("segment contains: \"%s\"\n", data);

	if (shmdt(data) == -1) {
		perror("shmdt");
		exit(1);
	}

	return 0;
}
