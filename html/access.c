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
