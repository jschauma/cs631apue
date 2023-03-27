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

/* This simple program illustrates the use of the chmod(2) system call,
 * and how it can be used to set explicit/absolute modes or to selectively
 * enable individual modes.
 *
 * Set your umask to 077, create file and file1, then run this command.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>

int
main() {
	struct stat sbuf;

	if (stat("file", &sbuf) == -1) {
		perror("can't stat file");
		exit(EXIT_FAILURE);
	}

	/* turn off owner read permissions and turn on setgid */
	if (chmod("file", (sbuf.st_mode & ~S_IRUSR) | S_ISGID) == -1) {
		perror("can't chmod file");
		exit(EXIT_FAILURE);
	}

	/* set absolute mode to rw-r--r-- */
	if (chmod("file1", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
		perror("can't chmod file1");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
