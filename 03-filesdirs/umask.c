/* This simple program illustrates the use of a umask.
 *
 * First, set your umask to 022 and 'touch file'; the mode will be 0644.
 *
 * Next, run this program and find file1 with the same mode, but execute
 * permissions added, file2 with the explicitly requested 0777 mode, and
 * finally file3 with mode 0711, since we explicitly turned off other
 * permissions.
 *
 * Observe that each time the flags passed to open(2) are identical.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
myOpen(const char *path) {
	int fd;
	if ((fd = open(path, O_CREAT | O_EXCL | O_WRONLY,
				S_IRUSR | S_IWUSR | S_IXUSR |
				S_IRGRP | S_IWGRP | S_IXGRP |
				S_IROTH | S_IWOTH | S_IXOTH)) == -1) {
		fprintf(stderr, "Unable to create %s: %s\n",
				path, strerror(errno));
		exit(EXIT_FAILURE);
	}
	(void)close(fd);
}

int
main() {
	/* current umask from your shell will apply to 'file1' */
	myOpen("file1");

	umask(0);
	/* turning all bits off means we will allow
	 * all the bits specified in open(2) */
	myOpen("file2");

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	/* turning group/other permissions off in the
	 * umask means they won't be set in open(2) */
	myOpen("file3");

	return EXIT_SUCCESS;
}
