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
 * This trivial program attempts to determine how many
 * open files a process can have.  It illustrates the
 * use of pre-processor directives and sysconf(3) to
 * identify a resource that can be changed at system
 * run time.
 *
 * This program also asks getconf(1) and explicitly
 * asks getrlimit(2).  Look at the source code for
 * getconf(1) and sysconf(3) to show how they are
 * implemented, as well as manual page and
 * implementation of getdtablesize(3).
 *
 * Run this once, then use 'ulimit -n' to change the
 * limit and run again.
 *
 * If you (as root) set 'ulimit -n unlimited', what
 * value will you get?  Why?
 *
 * What happens if you set this limit to, say, 0?
 * Why?
 */

#include <sys/resource.h>
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
countOpenFiles(int num) {
	struct stat stats;
	int count = 0;
	for (int i = 0; i < num; i++) {
		if (fstat(i, &stats) == 0) {
			printf("Currently open: fd #%d (inode %ld)\n", i,
					stats.st_ino);
			count++;
		}
	}

	return count;
}

void
openFiles(int num) {
	int count, fd;

	count = countOpenFiles(num);

	printf("Currently open files: %d\n", count);

	for (int i = count; i <= num ; i++) {
		if ((fd = open("/dev/null", O_RDONLY)) < 0) {
			if (errno == EMFILE) {
				printf("Opened %d additional files, then failed: %s (%d)\n", i - count, strerror(errno), errno);
				break;
			} else {
				fprintf(stderr, "Unable to open '/dev/null' on fd#%d: %s (errno %d)\n",
						i, strerror(errno), errno);
				break;
			}
		}
	}
}

int
main() {
	int openmax;
	struct rlimit rlp;

#ifdef OPEN_MAX
	printf("OPEN_MAX is defined as %d.\n", OPEN_MAX);
#else
	printf("OPEN_MAX is not defined on this platform.\n");
#endif


	printf("'getconf OPEN_MAX' says: ");
	(void)fflush(stdout);
	(void)system("getconf OPEN_MAX");

	/* see also: getdtablesize(3), notably the HISTORY section */
	errno = 0;
	if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
		if (errno == 0) {
			fprintf(stderr, "sysconf(3) considers _SC_OPEN_MAX unsupported?\n");
		} else {
			fprintf(stderr, "sysconf(3) error for _SC_OPEN_MAX: %s\n",
					strerror(errno));
		}
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}
	printf("sysconf(3) says this process can open %d files.\n", openmax);

	if (getrlimit(RLIMIT_NOFILE, &rlp) != 0) {
		fprintf(stderr, "Unable to get per process rlimit: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	openmax = (int)rlp.rlim_cur;
	printf("getrlimit(2) says this process can open %d files.\n", openmax);

	printf("Which one is it?\n\n");

	openFiles(openmax);

	return EXIT_SUCCESS;
}
