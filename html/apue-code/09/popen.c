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

/* This is basically the same as 'pipe2.c', but using
 * popen(3).  Make sure to illustrate the dangers of
 * invoking a shell on user-provided input.
 *
 * $ env PAGER="more; touch /tmp/boo" ./a.out popen.c
 * $ env PAGER="more; rm /etc/passwd 2>/dev/null"
 * ./a.out popen.c
 */

#include <sys/wait.h>

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define	DEF_PAGER	"/usr/bin/more"

int
main(int argc, char **argv) {
	FILE *pipe;
	char line[BUFSIZ], *pager;
	FILE *fp;

	if (argc != 2) {
		(void)fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if ((pager = getenv("PAGER")) == NULL)
		pager = DEF_PAGER;

	if ((pipe = popen(pager, "w")) == NULL) {
		(void)fprintf(stderr,"Unable to open pipe to %s: %s\n",
				pager, strerror(errno));
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		(void)fprintf(stderr, "can't open %s\n", argv[1]);
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	while (fgets(line, BUFSIZ, fp) != NULL) {
		(void)fprintf(pipe, "==> %s", line);
	}

	if (ferror(fp)) {
		err(EXIT_FAILURE, "fgets");
		/* NOTREACHED */
	}

	if (pclose(pipe) == -1) {
		err(EXIT_FAILURE, "pclose");
		/* NOTREACHED */
	}

	return EXIT_SUCCESS;
}
