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

/* This program illustrates the use of getpriority(2)
 * and setpriority(2).  It shows that we can adjust
 * our priority by lowering it, but that we can't
 * raise it again afterwards, unless we are root.
 *
 * Run as
 * $ ./a.out 5
 * $ sudo ./a.out 5
 * $ nice -n 10 ./a.out 5
 * $ nice -n 10 ./a.out 15
 * $ nice -n -5 ./a.out 5
 */

#include <sys/resource.h>

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char **argv) {
	int n, p1, p2;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s num\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* If argv[1] is not a valid number, then we
	 * get 0, but so be it, we'll use that. */
	n = atoi(argv[1]);

	errno = 0;
	if ((p1 = getpriority(PRIO_PROCESS, getpid())) == -1) {
		if (errno != 0) {
			err(EXIT_FAILURE, "getpriority");
			/* NOTREACHED */
		}
	}
	(void)printf("My current priority is: %d\n", p1);

	if (setpriority(PRIO_PROCESS, getpid(), n) == -1) {
		err(EXIT_FAILURE, "setpriority");
		/* NOTREACHED */
	}

	errno = 0;
	if ((p2 = getpriority(PRIO_PROCESS, getpid())) == -1) {
		if (errno != 0) {
			err(EXIT_FAILURE, "getpriority");
			/* NOTREACHED */
		}
	}
	(void)printf("My new priority is: %d\n", p2);

	/* We expect this to fail when running with
	 * euid != 0, since we can't raise our
	 * priority even to a value we initially had.
	 */
	if (setpriority(PRIO_PROCESS, getpid(), p1) == -1) {
		(void)fprintf(stderr, "Unable to setpriority(): %s\n", strerror(errno));
	}

	errno = 0;
	if ((p2 = getpriority(PRIO_PROCESS, getpid())) == -1) {
		if (errno != 0) {
			err(EXIT_FAILURE, "getpriority");
			/* NOTREACHED */
		}
	}
	(void)printf("My priority %sis: %d\n", (p1 != p2) ? "still " : "", p2);
	return 0;
}
