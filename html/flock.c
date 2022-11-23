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

/* This program illustrates the use of flock(2).  Run two concurrent
 * processes to show how a shared lock can be applied by both, but only
 * one process can upgrade to an exclusive lock.
 *
 * Illustrate blocking and non-blocking modes: in blocking mode, we
 * are avoiding the danger of a deadlock by the fact that upgrading a read
 * lock releases that lock, allowing the second process to eventually get
 * the exlusive lock.  In non-blocking mode, we avoid a deadlock by
 * explicitly releasing all locks and aborting after 10 tries.
 */
#include <sys/file.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
progress() {
	for (int i=0; i < 10; i++) {
		/* Using write(2), because printf(3) is line-buffered. */
		(void)write(STDOUT_FILENO, ".", 1);
		sleep(1);
	}
}

int
main(int argc, char **argv) {
	int i, fd, flags;

	(void)argv;

	if ((fd = open("/tmp/1", O_CREAT|O_RDWR, 0644)) == -1) {
		(void)fprintf(stderr, "Unable to open /tmp/1: %s", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if (flock(fd, LOCK_SH) < 0) {
		perror("flocking");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	(void)printf("Shared lock established - sleeping for 10 seconds.\n");
	progress();
	(void)printf("\nNow trying to get an exclusive lock.\n");

	flags = LOCK_EX;
	if (argc > 1) {
		flags |= LOCK_NB;
	}

	for (i=0; i < 10; i++) {
		if (flock(fd, flags) < 0) {
			(void)printf("Unable to get an exclusive lock.\n");
			if (i==9) {
				(void)printf("Giving up all locks.\n");
				flock(fd, LOCK_UN);
				exit(EXIT_FAILURE);
				/* NOTREACHED */
			}
			sleep(1);
		} else {
			(void)printf("Exclusive lock established.\n");
			break;
		}
	}

	progress();
	(void)printf("\n");
	(void)close(fd);
	exit(EXIT_SUCCESS);
}
