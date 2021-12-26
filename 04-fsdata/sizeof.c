/* Students frequently think that the result of 'sizeof'
 * is equivalent to that of strlen(3).  Let's illustrate
 * the differences. */

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	printf("int: %ld\n", sizeof(int));

	char buf[BUFSIZ];
	printf("Fixed buffer, size known at compile time: \"buf[%d]\"\n", BUFSIZ);
	printf("strlen: %lu - sizeof: %lu\n\n", strlen(buf), sizeof buf);

	char buf2[2] = { 'a' };
	printf("Fixed-size, initialized buffer: \"buf[2] = { 'a' }\"\n");
	printf("strlen: %lu - sizeof: %lu\n\n", strlen(buf2), sizeof buf2);

	char *s = "abcde";
	printf("A pointer: 'char *s = \"abcde\"'\n");
	printf("strlen: %lu - sizeof: %lu\n\n", strlen(s), sizeof s);

	char buf3[] = "abcde";
	printf("Another initialized buffer, implicitly sized: 'buf3[] =\"abcde\"'\n");
	printf("strlen: %lu - sizeof: %lu\n\n", strlen(buf3), sizeof buf3);

	if ((s = malloc(BUFSIZ)) == NULL) {
		fprintf(stderr, "Unable to allocate memory: %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("A pointer to malloc(3)'d memory:\n");
	printf("strlen: %lu - sizeof: %lu\n\n", strlen(s), sizeof s);

	(void)strncat(s, "0123456789", BUFSIZ);
	printf("After strncat(3)ing \"0123456789\" to 's':\n");
	printf("strlen: %lu - sizeof: %lu\n\n", strlen(s), sizeof s);

	return EXIT_SUCCESS;
}
