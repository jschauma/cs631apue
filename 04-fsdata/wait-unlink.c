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

/* This simple program is used to illustrate that the OS does not free up
 * the disk space used by a file immediately after a call to unlink(2),
 * but only after the last process with an open handle has terminated.
 *
 * Run like this:
 *
 * $ cc -Wall wait-unlink.c
 * $ ./a.out
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

	/* "/tmp" is a memory file system on our
	 * reference platform, so let's use a separate
	 * temporary directory backed by a "normal"
	 * filesystem. */
	if (chdir("/var/tmp") == -1) {
		perror("unable to cd to /var/tmp");
		exit(EXIT_FAILURE);
	}

	runDf();

	/* Hit return to continue. */
	(void)getchar();

	printf("Creating a 500M file...\n");
	if (system("dd if=/dev/zero of=foo bs=1024k count=500 >/dev/null") != 0) {
		perror("unable to dd a new file");
		exit(EXIT_FAILURE);
	}

	runDf();

	(void)getchar();
	printf("\nLinking 'bar' to 'foo'...\n\n");
	if (link("foo", "bar") == -1) {
		perror("unable to create a second hard link");
		exit(EXIT_FAILURE);
	}

	if (system("ls -li foo bar") != 0) {
		perror("unable to run ls(1)");
		exit(EXIT_FAILURE);
	}

	runDf();

	(void)getchar();
	if ((fd = open("foo", O_RDWR)) < 0) {
		perror("can't open file");
		exit(EXIT_FAILURE);
	}
	printf("\nOpened 'foo' on fd#%d.\n", fd);

	(void)getchar();
	if (unlink("foo") < 0) {
		perror("error unlinking");
		exit(EXIT_FAILURE);
	}

	printf("\nOk, foo unlinked.\n");
	printf("Disk space not free'd since 'bar' still exists...\n");
	/* We expect system(3) to fail here, since 'foo' no longer exists. */
	(void)system("ls -li foo bar");

	runDf();

	(void)getchar();
	printf("\nOk, now unlinking 'bar'...\n");
	if (unlink("bar") < 0) {
		perror("error unlinking");
		exit(EXIT_FAILURE);
	}

	printf("\nOk, bar unlinked.\n");
	(void)getchar();

	printf("Disk space not free'd since I still have fd#%d open...\n", fd);
	printf("\nRunning 'ls -li foo bar':\n");
	(void)system("ls -li foo bar");

	runDf();

	(void)getchar();
	printf("Now closing fd#%d...\n", fd);
	/* Closing the file descriptor after having
	 * unlinked all references to the 500M file
	 * finally frees the disk space. */
	(void)close(fd);
	printf("\n...and done.  Disk space is freed now.\n");

	runDf();

	exit(EXIT_SUCCESS);
}
