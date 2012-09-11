/*
 * This trivial program attempts to determine how many open file
 * descriptors a process can have.  It illustrates the use of
 * pre-processor directives and sysconf(3) to identify a resource that can
 * be changed at system run time.
 */

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

#define OPEN_MAX_GUESS 256

int
open_max(void) {
#ifdef OPEN_MAX
	static int openmax = OPEN_MAX;
#else
	static int openmax = 0;
#endif

	if (openmax == 0) {
		errno = 0;
		if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
			if (errno == 0)
				openmax = OPEN_MAX_GUESS;	/* indeterminate */
			else
				fprintf(stderr, "sysconf error for _SC_OPEN_MAX\n");
		}
	}

	return openmax;
}

int
main(int argc, char **argv) {
	printf("I could have %d open file descriptors.\n", open_max());
	return 0;
}
