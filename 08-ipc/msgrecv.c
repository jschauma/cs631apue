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

/* A simple program to illustrate the use of Message
 * Queues.
 *
 * Note that message queues continue to exist after
 * all processes have terminated; messages continue to
 * remain in the queues as well.  This is desired, but
 * requires that processes clean up after themselves
 * when they are done using the queues.
 *
 * Note that in this example we manually specify the
 * key instead of using ftok(2).
 *
 * Use msgsend.c to create/send messages, then run
 * this tool to retrieve them (in order).  Note that
 * msgrecv will block if no messages are in the queue.
 *
 * Use ipcs(1) to inspect the usage.
 *
 * Derived from:
 * https://users.cs.cf.ac.uk/Dave.Marshall/C/node25.html
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
		(void)fprintf(stderr, "Usage: msgrecv key\n");
		exit(EXIT_FAILURE);
	}

	if ((key = atoi(argv[1])) < 1) {
		(void)fprintf(stderr, "Invalid key: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if ((msqid = msgget(key, 0)) < 0) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}

	if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
		perror("msgrcv");
		exit(EXIT_FAILURE);
	}

	(void)printf("%s\n", rbuf.mtext);
	exit(EXIT_SUCCESS);
}
