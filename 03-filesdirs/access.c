/* This trivial program illustrates the use of access(2).  Compile this
 * program and run it with /etc/passwd, /etc/master.passwd as arguments;
 * then chown and setuid the executable and repeat.
 *
 * After chowning and setuid'ing the executable (ie a.out), note that
 * trying to compile the program again may give unexpected results on Mac
 * OS X:  ld will remove the executable after failing to write to it.
 * NetBSD and Linux will also remove the executable, but then create a new
 * one.  Use this to illustrate the fact that nuking a file does not
 * require write permissions on the file itself, but on the directory.
 * Also illustrates the use of dtrace(1), ktrace(8), strace(1) etc.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char **argv) {

	if (argc != 2) {
		fprintf(stderr, "%s: usage: %s filename\n", argv[0], argv[0]);
		exit(1);
	}

	if (access(argv[1], R_OK) == -1)
		printf("access error for %s\n", argv[1]);
	else
		printf("access ok for %s\n", argv[1]);


	if (open(argv[1], O_RDONLY) == -1)
		printf("open error for %s\n", argv[1]);
	else
		printf("open ok for %s\n", argv[1]);

	exit(0);
}
