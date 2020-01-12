#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MQ_PATH "/mq-example"
#define MQ_DEFAULT_PRIORITY 0

int
main(int argc, char **argv) {
	mqd_t mq;
	int i;

	setprogname(argv[0]);
	if ((mq = mq_open(MQ_PATH, O_WRONLY | O_CREAT)) == (mqd_t)-1) {
		fprintf(stderr, "%s: Unable to open message queue: %s\n",
				getprogname(), strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (i = 1; i < argc; i++) {
		if (mq_send(mq, argv[i], strlen(argv[i]), MQ_DEFAULT_PRIORITY) == -1) {
			fprintf(stderr, "%s: Unable to send message %d: %s\n",
					getprogname(), i, strerror(errno));
		}
#ifdef WAIT
		sleep(1);
#endif
	}

	if (mq_send(mq, "semi-urgent", strlen("semi-urgent"), 5) == -1) {
		fprintf(stderr, "%s: Unable to send urgent message: %s\n",
					getprogname(), strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (mq_send(mq, "urgent", strlen("urgent"), 10) == -1) {
		fprintf(stderr, "%s: Unable to send urgent message: %s\n",
					getprogname(), strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (mq_close(mq) == -1) {
		fprintf(stderr, "%s: Unable to close message queue: %s\n",
					getprogname(), strerror(errno));
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
