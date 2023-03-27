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

/* A simple program to illustrate the difference
 * between effective and real user IDs.
 *
 * Compile, then run as user1 and user2:
 * user1$ cc myuids.c
 * user1$ ./a.out
 * user2$ ./a.out
 * user1$ chmod u+s a.out
 * user1$ ./a.out
 * user2$ ./a.out
 *
 * For a more complete, albeit notably more complex
 * illustration of the various uids and the handling
 * of saved-setuid bits, see setuid.c and the comments
 * and links in there.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
printUids() {
	uid_t ruid = getuid();
	uid_t euid = geteuid();

	printf("My effective uid is: %d\n", euid);
	printf("My real uid is: %d\n", ruid);
}

int
main() {
	printUids();

	uid_t ruid = getuid();
	uid_t euid = geteuid();
	if (ruid == euid) {
		/* Nothing meaningful to do here. */
		exit(EXIT_SUCCESS);
		/* NOTREACHED */
	}

	printf("\nLet's drop elevated privs and set our effective uid to our real uid.\n");
	printf("Calling 'seteuid(%d)'...\n", ruid);
	if (seteuid(ruid) < 0) {
		fprintf(stderr, "Unable to seteuid(%d): %s\n", ruid, strerror(errno));
	} else {
		printUids();
	}

	printf("\nBecause we used seteuid(%d) and not setuid(%d), we can get back our earlier euid %d.\n", ruid, ruid, euid);
	printf("Calling 'seteuid(%d)'...\n", euid);
	if (seteuid(euid) < 0) {
		fprintf(stderr, "Unable to seteuid(%d): %s\n", euid, strerror(errno));
	} else {
		printUids();
	}

	printf("\nNow let's try to 'setuid(%d)'...\n", ruid);
	if (setuid(ruid) < 0) {
		fprintf(stderr, "Unable to setuid(%d): %s\n", ruid, strerror(errno));
	} else {
		printUids();
	}

	printf("\nNow let's try to gain back our effective uid %d...\n", euid);
	printf("Calling 'seteuid(%d)'...\n", euid);
	if (seteuid(euid) < 0) {
		fprintf(stderr, "Unable to seteuid(%d): %s\n", euid, strerror(errno));
	}
	printUids();

	return EXIT_SUCCESS;
}
