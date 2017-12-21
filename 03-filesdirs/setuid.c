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

int
main(int argc, char **argv) {
	uid_t ruid;
	uid_t euid;
	int fd;

	if (argc != 2) {
		fprintf(stderr, "%s: Usage: %s filename\n", argv[0], argv[0]);
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	ruid = getuid();
	euid = geteuid();

	printf("Program start: uid %d, euid %d\n", getuid(), geteuid());

	printf("We're privileged; let's set all UIDs to another account.\n");
	if (seteuid(UNPRIVILEGED_UID) == -1) {
		fprintf(stderr, "Unable to seteuid: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}
	printf("After seteuid(%d): uid %d, euid %d\n", UNPRIVILEGED_UID, getuid(), geteuid());

	printf("We're unprivileged, but with the help of the saved set-uid, we can regain root privs.\n");
	if (setuid(euid) == -1) {
		fprintf(stderr, "Unable to setuid: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}
	printf("After setuid(%d): uid %d, euid %d\n", euid, getuid(), geteuid());



	/* Immediately drop elevated privileges until
	 * we actually need them. */
	if (seteuid(ruid) == -1) {
		fprintf(stderr, "Unable to seteuid: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	printf("We dropped suid, now using: uid %d, euid %d\n", getuid(), geteuid());
	printf("Trying to open a privileged file...\n");

	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to open %s: %s\n",
					argv[1], strerror(errno));
	}

	/* We don't really do anything with the file
	 * handle, so let's just close it right away.
	 * We're once again willfully ignoring the
	 * error. */
	(void)close(fd);

	printf("Ok, let's try with elevated privileges.\n");

	if (seteuid(euid) == -1) {
		fprintf(stderr, "Unable to seteuid: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	printf("After seteuid(%d): uid %d, euid %d\n", euid, getuid(), geteuid());

	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to open %s: %s\n",
					argv[1], strerror(errno));
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

	printf("After setuid(%d): uid %d, euid %d\n", ruid, getuid(), geteuid());

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
	if (seteuid(euid) != -1) {
#ifndef _POSIX_SAVED_IDS
		if (euid == 0)
#endif
			fprintf(stderr, "seteuid(0) should not have succeeded!\n");
	}

	printf("After attempted seteuid(%d): uid %d, euid %d\n", euid, getuid(), geteuid());

	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "Unable to open %s: %s\n",
					argv[1], strerror(errno));
	} else {
		(void)close(fd);
	}

	exit(EXIT_SUCCESS);
}
