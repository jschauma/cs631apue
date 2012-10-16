/* A simple program to illustrate the use of Message Queues.
 * Note that message queues continue to exist after all processes have
 * terminated; messages continue to remain in the queues as well.  This is
 * desired, but requires that processes clean up after themselves when
 * they are done using the queues.
 *
 * Note that in this example we manually specify the key instead of using
 * ftok(2).
 *
 * Use msgsend.c to create/send messages, then run this tool to retrieve
 * them (in order).
 *
 * Use ipcs(1) to inspect the usage.
 *
 * Derived from:
 * http://www.cs.cf.ac.uk/Dave/C/node25.html
 */
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGSZ     128

typedef struct msgbuf {
	long    mtype;
	char    mtext[MSGSZ];
} message_buf;


int
main(int argc, char **argv) {
	int msqid;
	key_t key;
	message_buf rbuf;

	if (argc != 2) {
		fprintf(stderr, "Usage: msgrecv key\n");
		exit(EXIT_FAILURE);
	}

	if ((key = atoi(argv[1])) < 1) {
		fprintf(stderr, "Invalid key: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if ((msqid = msgget(key, 0666)) < 0) {
		perror("msgget");
		exit(1);
	}

	/*
	 * Receive an answer of message type 1.
	 */
	if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
		perror("msgrcv");
		exit(EXIT_FAILURE);
	}

	printf("%s\n", rbuf.mtext);
	exit(EXIT_SUCCESS);
}
