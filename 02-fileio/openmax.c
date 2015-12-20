/*
 * This trivial program attempts to determine how many open file
 * descriptors a process can have.  It illustrates the use of
 * pre-processor directives and sysconf(3) to identify a resource that can
 * be changed at system run time.
 *
 * This version also displays the per-process limit
 * via getrlimit(2) to show that resources may be
 * limited per process or per user.
 */

#include <sys/resource.h>

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	struct rlimit rlp;
	if (getrlimit(RLIMIT_NOFILE, &rlp) != 0) {
		fprintf(stderr, "Unable to get per process rlimit: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("This user could open %d file descriptors.\n", open_max());
	printf("This process could open %d file descriptors.\n", (int)rlp.rlim_cur);
	return EXIT_SUCCESS;
}
