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

int
main(void) {
	int i, fd, flags;

	if ((fd = open("/tmp/1", O_CREAT|O_RDWR, 0644)) == -1) {
		fprintf(stderr, "Unable to open /tmp/1: %s", strerror(errno));
		exit(1);
	}

	if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
		perror("getting file flags");
		exit(1);
	}

	if (flock(fd, LOCK_SH) < 0) {
		perror("flocking");
		exit(1);
	}

	printf("Shared lock established - sleeping for 10 seconds.\n");
	for (i=0;i<10;i++) {
		fprintf(stderr, ".");
		sleep(1);
	}
	fprintf(stderr, "\n");
	printf("Now trying to get an exclusive lock.\n");

	for (i=0; i < 10; i++) {
#ifdef NONBLOCK
		if (flock(fd, LOCK_EX|LOCK_NB) < 0) {
#else
		if (flock(fd, LOCK_EX) < 0) {
#endif
			printf("Unable to get an exclusive lock.\n");
			if (i==9) {
				printf("Giving up all locks.\n");
				flock(fd, LOCK_UN);
				exit(1);
			}
			sleep(1);
		} else {
			printf("Exclusive lock established.\n");
			break;
		}
	}
	for (i=0;i<10;i++) {
		fprintf(stderr, ".");
		sleep(1);
	}
	fprintf(stderr, "\n");
	exit(0);
}
