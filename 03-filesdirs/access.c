/* This trivial program illustrates the use of access(2).  Compile this
 * program and run it with /etc/passwd, /etc/master.passwd as arguments;
 * then chown and setuid the executable and repeat.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char **argv) {

	if (argc != 2) {
		fprintf(stderr, "%s: usage: %s filename\n", argv[0], argv[0]);
		exit(EXIT_FAILURE);
	}

	if (access(argv[1], R_OK) == -1)
		printf("access error for %s\n", argv[1]);
	else
		printf("access ok for %s\n", argv[1]);


	if (open(argv[1], O_RDONLY) == -1)
		printf("open error for %s\n", argv[1]);
	else
		printf("open ok for %s\n", argv[1]);

	exit(EXIT_SUCCESS);
}
