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

/* This program is used together with 'mqrecv' program
 * to illustrate the use of POSIX message queues; see
 * mq(3) for details.
 *
 * Remember to link with '-lrt'.
 *
 * Note: you need to run the 'mqrecv' program first,
 * as it create the message queue.
 */
#include <err.h>
#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MQ_PATH             "/sandwiches"
#define MQ_DEFAULT_PRIORITY 0
#define MQ_IMPORTANT        1

int
main(int argc, char **argv) {
	mqd_t mq;
	int ch, wait;

	wait = 0;
	setprogname(argv[0]);

	while ((ch = getopt(argc, argv, "w")) != -1) {
		switch (ch) {
		case 'w':
			wait = 1;
			break;
		default:
			(void)fprintf(stderr, "Usage: %s [-w] message\n",
					getprogname());
			exit(EXIT_FAILURE);
			/* NOTREACHED */
		}
	}
	argc -= optind;
	argv += optind;

	if ((mq = mq_open(MQ_PATH, O_WRONLY)) == (mqd_t)-1) {
		err(EXIT_FAILURE, "mq_open");
		/* NOTREACHED */
	}

	for (int i = 0; i < argc; i++) {
		if (mq_send(mq, argv[i], strlen(argv[i]), MQ_DEFAULT_PRIORITY) == -1) {
			(void)fprintf(stderr, "%s: Unable to send message %d: %s\n",
					getprogname(), i, strerror(errno));
		}

		if (wait) {
			sleep(1);
		}
	}

	if (mq_send(mq, "TUNA", strlen("TUNA"), MQ_IMPORTANT) == -1) {
		err(EXIT_FAILURE, "mq_send");
		/* NOTREACHED */
	}

	if (mq_close(mq) == -1) {
		err(EXIT_FAILURE, "mq_close");
		/* NOTREACHED */
	}

	return EXIT_SUCCESS;
}
