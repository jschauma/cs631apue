#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MQ_PATH "/mq-example"
#define MQ_FLAGS O_RDONLY | O_CREAT | O_EXCL | O_NONBLOCK
#define MQ_PERMS S_IRUSR | S_IWUSR
#define MQ_MAXMSG 10
#define MQ_MSGSIZE 1024
#define MQ_DEFAULT_PRIORITY 0

int msgreceived;

void
emptyQueue(mqd_t mq, struct sigevent se) {
	if (msgreceived == 1) {
		unsigned prio;
		char buf[BUFSIZ];

		struct mq_attr attr;
		if (mq_getattr(mq, &attr) == -1) {
			fprintf(stderr, "Unable to get mq attributes: %s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
		printf("Number of messages in queue: %ld\n", attr.mq_curmsgs);

		/* Note: we only get notified if the message queue is
		 * empty and it receives a message; if we don't pick them
		 * off right away, they are queued, so we drain the queue
		 * completely here.
		 *
		 * Note also: If a message of a higher priority comes in,
		 * it'll be picked off first. */
		while (mq_receive(mq, buf, BUFSIZ, &prio) > 0) {
			printf("Message of priority %d: %s\n", prio, buf);
			bzero(buf, BUFSIZ);
		}
		if ((errno > 0) && (errno != EAGAIN)) {
			fprintf(stderr, "Unable to receive message from queue: %s\n",
				strerror(errno));
		}

		/* After every notification, the registration to be
		 * notified is removed, so we need to re-register. */
		if (mq_notify(mq, &se) == -1) {
			fprintf(stderr, "Unable to set up mq notification: %s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	msgreceived = 0;
}

void
msghandler() {
	msgreceived = 1;
}

int
main(int argc, char **argv) {
	struct mq_attr attr;
	mqd_t mq;

	struct sigevent se;

	setprogname(argv[0]);

	/* If we exited uncleanly, we may not have
	 * removed the message queue, so try to unlink
	 * it here.  If that fails, chances are it
	 * didn't exist, and no harm is done. */
	(void)mq_unlink(MQ_PATH);

	if (signal(SIGUSR1, msghandler) == SIG_ERR) {
		fprintf(stderr, "%s: Can't catch SIGUSR1: %s", getprogname(), strerror(errno));
		exit(EXIT_FAILURE);
	}

	attr.mq_flags = 0;
	attr.mq_maxmsg = MQ_MAXMSG;
	attr.mq_msgsize = MQ_MSGSIZE;
	attr.mq_curmsgs = 0;

	if ((mq = mq_open(MQ_PATH, MQ_FLAGS, MQ_PERMS, &attr)) == (mqd_t)-1) {
		fprintf(stderr, "%s: Unable to create message queue: %s\n",
				getprogname(), strerror(errno));
		exit(EXIT_FAILURE);
	}

	se.sigev_notify = SIGEV_SIGNAL;
	se.sigev_signo = SIGUSR1;

	if (mq_notify(mq, &se) == -1) {
		fprintf(stderr, "%s: Unable to set up mq notification: %s\n",
				getprogname(), strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (;;) {
		pause();
		if (argc > 1) {
			/* Let's give the client some time to deliver
			 * multiple messages. */
			sleep(5);
		}
		emptyQueue(mq, se);
	}

	if (mq_close(mq) == -1) {
		fprintf(stderr, "%s: Unable to close message queue: %s\n",
				getprogname(), strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (mq_unlink(MQ_PATH) == -1) {
		fprintf(stderr, "%s: Unable to delete message queue: %s\n",
				getprogname(), strerror(errno));
		exit(EXIT_FAILURE);
	}
}
