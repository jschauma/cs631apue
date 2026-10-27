/* This file is part of the sample code and exercises
 * used by the class "Advanced Programming in the UNIX
 * Environment" taught by Jan Schaumann
 * <jschauma@netmeister.org> at Stevens Institute of
 * Technology.
 *
 * This file is in the public domain.
 *
 * Conceptually based on the code examples from
 * https://en.wikipedia.org/wiki/Setjmp.h and
 * https://www.codewithc.in/tutorials/setjmp-longjmp
 *
 * You don't have to, but if you feel like
 * acknowledging where you got this code, you may
 * reference me by name, email address, or point
 * people to the course website:
 * https://stevens.netmeister.org/631/
 *
 * This program illustrates how you might try to
 * emulate exception handling using the
 * setjmp(3)/longjmp(3) functions.  However, just
 * because you _can_ doesn't mean it's a good idea.
 *
 */

#include <err.h>
#include <errno.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static jmp_buf stack;

#define TRY if ((e = setjmp(stack)) == 0)
#define CATCH(err) else
#define THROW(err) longjmp(stack, err)

#define FILE_IO_EXCEPTION 1
#define MEMORY_EXCEPTION 2
#define UNEXPECTED_EXCEPTION 3

void doFileStuff(const char *f) {
	int i;

	srandom(time(0));
	i = random()%3;

	if (access(f, F_OK) < 0) {
		THROW(FILE_IO_EXCEPTION);
	}

	printf("File access ok, let's do the thing.\n");
	/* simulate some memory error */
	if (i == 1) {
		errno = ENOMEM;
		THROW(MEMORY_EXCEPTION);
	}

	if (i == 2) {
		errno = EPROTO;
		THROW(UNEXPECTED_EXCEPTION);
	}

	printf("doFileStuff completed.\n");
}

int main(int argc, char **argv)
{
	int e = 0;

	setprogname(argv[0]);

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", getprogname());
		exit(EXIT_FAILURE);
	}

	printf("Let's go!\n");
	TRY {
		doFileStuff(argv[1]);
		printf("Nice.\n");
	}
	CATCH(e) {
		if (e == FILE_IO_EXCEPTION) {
			warn("Unable to do file stuff");
		} else if (e == MEMORY_EXCEPTION) {
			err(EXIT_FAILURE, "Can't go on after memory failure:");
			/* NOTREACHED */
		} else {
			fprintf(stderr, "Any other exception: %s\n", strerror(errno));
		}
	}

	if (e) {
		printf("We gracefully recovered from any non-fatal errors.\n");
	} else {
		printf("Everything went according to plan.\n");
	}

	return EXIT_SUCCESS;
}
