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

/* This trivial program illustrates the use of
 * setuid(2) and seteuid(2).  After compiling,
 * chmod(1) and chown(1) as needed to access a
 * privileged resource (e.g. /etc/sudoers).
 *
 * Note: this example ignores group IDs.  See
 * https://is.gd/l6pu5K for details on the
 * order of dropping all privileges correctly.
 *
 * Example invocation:
 * cc -Wall setuid.c
 * sudo chown root a.out
 * sudo chmod 4755 a.out
 * ./a.out /etc/sudoers
 *
 * Note: after chowning, try to recompile.  Does
 * your compiler overwrite a.out?  Why/why not?
 *
 * Note: setuid is not restricted to setuid-0.  Try
 * with another user!
 *
 * mkdir -m 0700 /tmp/daemon
 * touch /tmp/daemon/somefile
 * chmod a+r /tmp/daemon/somefile
 * sudo chown daemon /tmp/daemon
 * sudo chown daemon a.out
 * sudo chmod 4755 a.out
 * ls -l a.out
 * ls -l /tmp/daemon
 * ls -ld /tmp/daemon
 * ./a.out /tmp/daemon/somefile
 * ./a.out /etc/sudoers
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UNPRIVILEGED_UID 1

char buf[BUFSIZ];

uid_t ruid;
uid_t euid;
uid_t suid;

void myseteuid(int);
void printUids(const char *);

/* We're using this wrapper function, because the behavior
 * of seteuid(2) with respect to the saved-set-uid is inconsistent
 * across platforms.  On e.g. NetBSD, the POSIX.1-2017 mandated
 * behavior is not implemented; see the note in the manual page
 * as well as in <unistd.h>. */
void
myseteuid(int myeuid) {
	char *func = "seteuid(";
#ifdef _POSIX_SAVED_IDS
	if (seteuid(myeuid) == -1) {
		fprintf(stderr, "Unable to seteuid(%d): %s\n", myeuid, strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}
#else
	if (setreuid(-1, myeuid) == -1) {
		fprintf(stderr, "Unable to setreuid(-1, %d): %s\n", myeuid, strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}
	func = "setreuid(-1, ";
#endif
	if (snprintf(buf, BUFSIZ, "After %s%d)", func, myeuid) < 0) {
		fprintf(stderr, "Unable to snprintf: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}
	printUids(buf);
}

/* On e.g. Linux, we can get the saved set-uid
 * via getresuid(2); not all systems support
 * this, however.  On those that do not,
 * we use the suid value initialized at program
 * startup.
 */
void
printUids(const char *msg) {
	ruid = getuid();
	euid = geteuid();

#ifdef _GNU_SOURCE
	if (getresuid(&ruid, &euid, &suid) < 0) {
		fprintf(stderr, "Unable to getresuid: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}
#endif
	printf("%s: ruid %d, euid %d, suid %d\n", msg, ruid, euid, suid);

}

int
main(int argc, char **argv) {
	int fd;

	if (argc != 2) {
		fprintf(stderr, "%s: Usage: %s filename\n", argv[0], argv[0]);
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	ruid = getuid();
	euid = geteuid();
	/* fake saved-setuid */
	suid = geteuid();

	printUids("Program start");

	printf("We're privileged; let's set all UIDs to another account.\n");
	myseteuid(UNPRIVILEGED_UID);

	printf("We're unprivileged, but with the help of the saved set-uid, we can regain initial setuid (%d) privs.\n", suid);

	printf("But let's drop them again via seteuid(%d)...\n", ruid);
	myseteuid(ruid);

	printf("Trying to open a privileged file...\n");
	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to open %s: %s\n",
					argv[1], strerror(errno));
	}
	/* We don't do anything with the file, we just demonstrated
	 * that we were unable to open it. */
	(void)close(fd);

	printf("Ok, let's try with elevated privileges.\n");
	myseteuid(suid);

	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to open %s: %s\n",
					argv[1], strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	} else {
		printf("Opening worked.\n");

		/* Now we could do stuff with 'fd', if we were
		 * so inclined.  We're not, though. */
		(void)close(fd);
	}

	printf("Alright, we're done using our elevated privileges.  Let's drop them permanently.\n");
	if (setuid(ruid) == -1) {
		fprintf(stderr, "Unable to setuid: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if (snprintf(buf, BUFSIZ, "After setuid(%d)", ruid) < 0) {
		fprintf(stderr, "Unable to snprintf: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}
	printUids(buf);
	printf("Now trying to gain elevated privileges again.\n");

	/* Trying to gain elevated privileges again
	 * should fail here, because setuid(2), called
	 * above, is supposed to set the real and effective
	 * uid as well as the saved set-user ID.
	 *
	 * However, the results are platform dependent,
	 * based on whether or not the euid at
	 * execution start was 0 or not.  See
	 * https://is.gd/QVO35q for details.
	 *
	 * In practice, we observe that:
	 * - NetBSD sets the saved set-user-ID in setuid(2)
	 *   even for non-root
	 * - OS X and Linux do NOT set the saved set-user-ID
	 *   in setuid(2) IF the euid was non-zero
	 */
	if (seteuid(suid) != -1) {
#ifdef _POSIX_SAVED_IDS
		if (euid == 0) {
			fprintf(stderr, "seteuid(%d) should not have succeeded!\n", euid);
		}
#endif
	}

	if (snprintf(buf, BUFSIZ, "After attempted seteuid(%d)", suid) < 0) {
		fprintf(stderr, "Unable to snprintf: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}
	printUids(buf);

	printf("(We expect the following call to open(2) to fail if setuid was 0 initially.)\n");

	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to open %s: %s\n",
					argv[1], strerror(errno));
	} else {
		if (euid == 0) {
			printf("Wait, what? This shouldn't have worked!\n");
		}
		(void)close(fd);
	}

	exit(EXIT_SUCCESS);
}
