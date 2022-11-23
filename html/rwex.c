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

/*
 * This trivial program illustrates how we might normally read and write
 * to a simple file. Here, we simply open the source code file, read the
 * first few bytes, then append a comment.
 *
 * What happens if we do not open with O_APPEND?
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE	64
#define COMMENT		"\n/* Just another comment. */\n"
#define SOURCE 		"./rwex.c"

int
main() {
	int fd, n;
	int len;
	char buf[BUFFSIZE];

	if ((fd = open(SOURCE, O_RDWR | O_APPEND)) == -1) {
		fprintf(stderr, "Unable to open '%s': %s\n",
				SOURCE, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if ((n = read(fd, buf, BUFFSIZE)) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			fprintf(stderr, "Unable to write: %s\n",
					strerror(errno));
			exit(EXIT_FAILURE);
		}
	} else if (n == -1) {
		fprintf(stderr, "Error reading from %s: %s\n",
				SOURCE, strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("\n\nOk, we read the first %d bytes. Now let's write something.\n", BUFFSIZE);

	len = sizeof(COMMENT) - 1;
	if (write(fd, COMMENT, len) != len) {
		fprintf(stderr, "Unable to write: %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	(void)close(fd);
	return EXIT_SUCCESS;
}

/* Just another comment. */
