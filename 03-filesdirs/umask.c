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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv) {
	if ( open("foo1", O_CREAT | O_EXCL | O_WRONLY,
				S_IRUSR | S_IWUSR |
				S_IRGRP | S_IWGRP |
				S_IROTH | S_IWOTH) == -1 ) {
		perror("create error for foo1");
		exit(EXIT_FAILURE);
	}


	umask(0);
	if ( open("foo2", O_CREAT | O_EXCL | O_WRONLY,
				S_IRUSR | S_IWUSR |
				S_IRGRP | S_IWGRP |
				S_IROTH | S_IWOTH) == -1 ) {
		perror("create error for foo2");
		exit(EXIT_FAILURE);
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if ( open("foo3", O_CREAT | O_EXCL | O_WRONLY,
				S_IRUSR | S_IWUSR |
				S_IRGRP | S_IWGRP |
				S_IROTH | S_IWOTH) == -1 ) {
		perror("create error for foo3");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
