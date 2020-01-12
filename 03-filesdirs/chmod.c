/* This simple program illustrates the use of the chmod(2) system call,
 * and how it can be used to set explicit/absolute modes or to selectively
 * enable individual modes.
 *
 * Set your umask to 077, create foo and foo1, then run this command.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>

int
main() {
	struct stat sbuf;

	if (stat("foo", &sbuf) == -1) {
		perror("can't stat foo");
		exit(EXIT_FAILURE);
	}

	/* turn off group execute and turn on set-UID */
	if (chmod("foo", (sbuf.st_mode & ~S_IXGRP) | S_ISUID) == -1) {
		perror("can't chmod foo");
		exit(EXIT_FAILURE);
	}

	/* set absolute mode to rw-r--r-- */
	if (chmod("foo1", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
		perror("can't chmod foo1");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
