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

/* This program is used together with 'mqsend' program
 * to illustrate the use of POSIX message queues; see
 * mq(3) for details.
 *
 * Remember to link with '-lrt'.
 *
 * Note: you need to run this program -- i.e., the
 * receiver -- first, as it is the program that create
 * the message queue.
 */

#include <sys/stat.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MQ_PATH             "/sandwiches"
#define MQ_FLAGS            O_RDONLY | O_CREAT | O_EXCL | O_NONBLOCK
#define MQ_PERMS            S_IRUSR | S_IWUSR
#define MQ_MAXMSG           10
#define MQ_MSGSIZE          1024
#define MQ_DEFAULT_PRIORITY 0

int msgreceived;

void
emptyQueue(mqd_t mq, struct sigevent se) {
	if (msgreceived == 1) {
		unsigned prio;
		char buf[BUFSIZ];

		struct mq_attr attr;
		if (mq_getattr(mq, &attr) == -1) {
			err(EXIT_FAILURE, "mq_getattr");
			/* NOTREACHED */
		}
		(void)printf("Number of messages in queue: %ld\n", attr.mq_curmsgs);

		/* Note: we only get notified if the message queue is
		 * empty and it receives a message; if we don't pick them
		 * off right away, they are queued, so we drain the queue
		 * completely here.
		 *
		 * Note also: If a message of a higher priority comes in,
		 * it'll be picked off first. */
		while (mq_receive(mq, buf, BUFSIZ, &prio) > 0) {
			(void)printf("Message of priority %d: %s\n", prio, buf);
			bzero(buf, BUFSIZ);
		}
		if ((errno > 0) && (errno != EAGAIN)) {
			(void)fprintf(stderr, "Unable to receive message from queue: %s\n",
				strerror(errno));
		}

		/* After every notification, the registration to be
		 * notified is removed, so we need to re-register. */
		if (mq_notify(mq, &se) == -1) {
			err(EXIT_FAILURE, "mq_notify");
			/* NOTREACHED */
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
		err(EXIT_FAILURE, "Unable to install signal handler");
		/* NOTREACHED */
	}

	attr.mq_flags = 0;
	attr.mq_maxmsg = MQ_MAXMSG;
	attr.mq_msgsize = MQ_MSGSIZE;
	attr.mq_curmsgs = 0;

	if ((mq = mq_open(MQ_PATH, MQ_FLAGS, MQ_PERMS, &attr)) == (mqd_t)-1) {
		err(EXIT_FAILURE, "mq_open");
		/* NOTREACHED */
	}

	se.sigev_notify = SIGEV_SIGNAL;
	se.sigev_signo = SIGUSR1;

	if (mq_notify(mq, &se) == -1) {
		err(EXIT_FAILURE, "mq_notify");
		/* NOTREACHED */
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
		err(EXIT_FAILURE, "mq_close");
		/* NOTREACHED */
	}

	if (mq_unlink(MQ_PATH) == -1) {
		err(EXIT_FAILURE, "mq_unlink");
		/* NOTREACHED */
	}
}
