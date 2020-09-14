/* This simple program illustrates the use of the chown(2) system call:
 *
 * touch file
 * ./a.out
 * ls -l file
 * sudo ./a.out
 * ls -l file
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* We'll see better ways to get a user's UID or a
 * group's GID in our next lecture. */
#define FRED 1001
#define USERS_GID 100
#define TTY_GID 4

void
mychown(const char *file, int uid, int gid) {
	if (chown(file, uid, gid) < 0) {
		fprintf(stderr, "Unable to chown(%s, %d, %d): %s\n",
				file, uid, gid, strerror(errno));
	} else {
		printf("Successfully chowned %s to %d:%d.\n",
				file, uid, gid);
	}
}

int
main(int argc, char **argv) {

	if (argc != 2) {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Let's try to take ownership of the file.
	 * That's not really a useful thing to try,
	 * since either it works out, in which case
	 * we already owned the file, or it'll fail.
	 *
	 * Unless, of course, we're root... */
	mychown(argv[1], getuid(), -1);

	/* Next, let's try to change ownership to
	 * Fred.  (Don't hold your breath, unless
	 * you're root.) */
	mychown(argv[1], FRED, -1);

	/* Next, let's 'chown: users'... */
	mychown(argv[1], -1, USERS_GID);

	/* ... and let's try 'chown:tty'. */
	mychown(argv[1], -1, TTY_GID);

	exit(EXIT_SUCCESS);
}
