/* This is basically the same as 'pipe2.c', but using popen(3).  Make sure
 * to illustrate the dangers of invoking a shell on user-provided input.
 *
 * $ env PAGER="more; touch /tmp/boo" ./a.out popen.c
 * $ env PAGER="more; rm /etc/passwd 2>/dev/null" ./a.out popen.c
 */

#include <sys/wait.h>

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
		perror("usage: a.out <pathname>");
		exit(EXIT_FAILURE);
	}

	if ((pager = getenv("PAGER")) == NULL)
		pager = DEF_PAGER;

	if ((pipe = popen(pager, "w")) == NULL) {
		fprintf(stderr,"Unable to open pipe to %s: %s\n",
				pager, strerror(errno));
		exit(EXIT_FAILURE);
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "can't open %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	while (fgets(line, BUFSIZ, fp) != NULL) {
		fprintf(pipe, "==> %s", line);
	}

	if (ferror(fp)) {
		perror("fgets error");
		exit(EXIT_FAILURE);
	}

	if (pclose(pipe) == -1) {
		perror("pclose error");
		exit(EXIT_FAILURE);
	}

	return 0;
}
