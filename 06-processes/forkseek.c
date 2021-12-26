/* This program illustrates that after fork(2), the
 * child has a copy of the file descriptors from the
 * parent pointing to the same file table entries,
 * meaning operations on the fd in one affect the
 * other. */

#include <sys/wait.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM 32

void
readData(int fd) {
	int n;
	char buf[NUM];
	off_t offset;

	if ((offset = lseek(fd, 0, SEEK_CUR)) < 0) {
		err(EXIT_FAILURE, "%d lseek error", getpid());
		/* NOTREACHED */
	}

	(void)printf("%d offset is now: %ld\n", getpid(), offset);
	if ((n = read(fd, buf, NUM)) < 0) {
		err(EXIT_FAILURE, "PID %d: read error", getpid());
		/* NOTREACHED */
	}
	/* We don't do anything with the data; we just
	 * wanted to illustrate that reading will
	 * advance the offset. */
}


int
main(int argc, char **argv) {
	int fd;
	pid_t pid;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		err(EXIT_FAILURE, "open error");
		/* NOTREACHED */
	}

	(void)printf("Starting pid is: %d\n", getpid());

	readData(fd);

	if ((pid = fork()) < 0) {
		err(EXIT_FAILURE, "fork error");
		/* NOTREACHED */
	} else if (pid == 0) {
		if (lseek(fd, NUM, SEEK_CUR) < 0) {
			err(EXIT_FAILURE, "child lseek error");
			/* NOTREACHED */
		}
		(void)printf("child %d done seeking\n", getpid());
		/* give the parent a chance to read */
		sleep(2);
		readData(fd);
	} else {
		/* give the child a chance to lseek */
		sleep(1);
		readData(fd);
	}

	(void)wait(NULL);
	(void)close(fd);
	return EXIT_SUCCESS;
}
