/*
 * This trivial program verifies whether or not STDIN is seekable.  Test
 * on a regular file, on a pipe or a fifo.
 */

#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(void) {
	if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1 )
		printf("cannot seek\n");
	else
		printf("seek OK\n");

	return EXIT_SUCCESS;
}
