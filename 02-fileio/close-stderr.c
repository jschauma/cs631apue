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

/*
 * This program illustrates that you an close the default
 * standard I/O filedescriptors.  To reopen, you then need
 * to explicitly open the terminal device and dup(2) the
 * descriptor onto it.
 *
 * Note: as of 2024-09-15, on macOS Sonoma 14.6.1, the
 * restored stderr does not seem to work.  There,
 * 	freopen("/dev/tty", "w+", stderr)
 * appears to be necessary.
 */

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main() {
	int fd;

	if ((fd = open("/dev/tty", O_WRONLY)) < 0) {
		err(EXIT_FAILURE, "Unable to open /dev/tty");
		/* NOTREACHED */
	}
	(void)printf("Opened /dev/tty on fd#%d.\n", fd);

	(void)printf("Closing STDERR_FILENO...\n");
	(void)close(STDERR_FILENO);

	(void)fprintf(stderr, "This message will not be shown.\n");
	if (dup2(fd, STDERR_FILENO) < 0) {
		/* stderr is still closed */
		(void)fprintf(stdout, "Unable to dup2: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	(void)fprintf(stderr, "Error messages back to normal.\n");

	(void)close(fd);
	return EXIT_SUCCESS;
}
