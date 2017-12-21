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
	char line[128], *pager;
	FILE *fp;

	if (argc != 2) {
		perror("usage: a.out <pathname>");
		exit(1);
	}

	if ((pager = getenv("PAGER")) == NULL)
		pager = DEF_PAGER;

	if ((pipe = popen(pager, "w")) == NULL) {
		fprintf(stderr,"Unable to open pipe to %s: %s\n",
				pager, strerror(errno));
		exit(1);
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "can't open %s\n", argv[1]);
		exit(1);
	}

	while (fgets(line, 128, fp) != NULL) {
		fprintf(pipe, "==> %s", line);
	}

	if (ferror(fp)) {
		perror("fgets error");
		exit(1);
	}

	if (pclose(pipe) == -1) {
		perror("pclose error");
		exit(1);
	}

	return 0;
}
