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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA "1234567890"

int main() {
	char buf1[] = "abc";
	char buf2[BUFSIZ];
	char buf3[BUFSIZ] = { 'a', 'b', 'c' };
	char *s1 = "abc";
	char *s2 = buf3;

	char *s3;
	if ((s3 = malloc(BUFSIZ)) == NULL) {
		fprintf(stderr, "Unable to allocate memory: %s\n",
				strerror(errno));
		exit(EXIT_FAILURE);
	}
	(void)strncat(s3, "0123456789", BUFSIZ);

	return EXIT_SUCCESS;
}
