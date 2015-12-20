/* This simple program illustrates the use of a umask.
 *
 * First, set your umask to 022 and create a file 'foo'; the mode will be
 * 0644.
 *
 * Next, run this program and find foo1 with the same mode, foo2 with the
 * explicitly requested 0666 mode and finally foo3 with mode 0600, since
 * we explicitly turned off other permissions.
 *
 * Observe that each time the flags passwd to open(2) are identical.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
myOpen(const char *path) {
	if ( open(path, O_CREAT | O_EXCL | O_WRONLY,
				S_IRUSR | S_IWUSR |
				S_IRGRP | S_IWGRP |
				S_IROTH | S_IWOTH) == -1 ) {
		fprintf(stderr, "Unable to create %s: %s\n",
				path, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

int
main(int argc, char **argv) {
	myOpen("foo1");

	umask(0);
	myOpen("foo2");

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	myOpen("foo3");

	return EXIT_SUCCESS;
}
