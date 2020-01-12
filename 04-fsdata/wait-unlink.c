/* This simple program is used to illustrate that the OS does not free up
 * the disk space used by a file immediately after a call to unlink(2),
 * but only after the last process with an open handle has terminated.
 *
 * Run like this:
 *
 * $ cc -Wall wait-unlink.c
 * $ ./a.out
 * $ df .
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
runDf() {
	printf("\nAvailable space is now:\n");
	if (system("df .") != 0) {
		perror("unable to run df(1)");
		exit(EXIT_FAILURE);
	}
}

int
main() {
	int fd;

	if (chdir("/var/tmp") == -1) {
		perror("unable to cd to /var/tmp");
		exit(EXIT_FAILURE);
	}

	runDf();

	printf("Hit return to create a 500M file...");
	(void)getchar();
	if (system("dd if=/dev/zero of=foo bs=1024k count=500 >/dev/null") != 0) {
		perror("unable to dd a new file");
		exit(EXIT_FAILURE);
	}

	runDf();

	printf("\nHit return to link 'bar' to 'foo'...");
	(void)getchar();
	if (link("foo", "bar") == -1) {
		perror("unable to create a second hard link");
		exit(EXIT_FAILURE);
	}

	if (system("ls -li foo bar") != 0) {
		perror("unable to run ls(1)");
		exit(EXIT_FAILURE);
	}

	runDf();

	printf("\nHit return to open and then unlink 'foo'...");
	(void)getchar();

	if ((fd = open("foo", O_RDWR)) < 0) {
		perror("can't open file");
		exit(EXIT_FAILURE);
	}

	if (unlink("foo") < 0) {
		perror("error unlinking");
		exit(EXIT_FAILURE);
	}

	printf("\nOk, foo unlinked.  Disk space not free'd since 'bar' still exists...\n");
	/* We expect system(3) to fail here, since foo no longer exists. */
	(void)system("ls -li foo bar");

	runDf();

	printf("\nHit return to unlink 'bar'...");
	(void)getchar();
	if (unlink("bar") < 0) {
		perror("error unlinking");
		exit(EXIT_FAILURE);
	}

	printf("\nOk, bar unlinked.  Disk space not free'd since I still have a file handle open...\n");
	printf("\nRunning 'ls -li foo bar':\n");
	(void)system("ls -li foo bar");

	runDf();

	printf("\nHit return to close the open file descriptor.");
	(void)getchar();
	/* Closing the file descriptor after having
	 * unlinked all references to the 500M file
	 * finally frees the disk space. */
	close(fd);
	printf("\n...and done.  Disk space is freed now.\n");

	runDf();

	exit(EXIT_SUCCESS);
}
