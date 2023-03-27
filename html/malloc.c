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

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MALLOC_SIZE 1024 * 1024

void
printMalloc(void *p, int s) {
	(void)printf("malloc/realloc %d\n", s);
	(void)printf("begins at 0x%12lX\n", (unsigned long)p);
	(void)printf("ends   at 0x%12lX\n", (unsigned long)p+s);
	(void)printf("\n");
}

int
main() {
	void *ptr;

	if ((ptr = malloc(BUFSIZ)) == NULL) {
		err(EXIT_FAILURE, "malloc");
		/* NOTREACHED */
	}
	printMalloc(ptr, BUFSIZ);

	/* shrink */
	if ((ptr = realloc(ptr, BUFSIZ / 2)) == NULL) {
		err(EXIT_FAILURE, "realloc");
		/* NOTREACHED */
	}
	printMalloc(ptr, BUFSIZ / 2);

	/* grow */
	if ((ptr = realloc(ptr, MALLOC_SIZE)) == NULL) {
		err(EXIT_FAILURE, "realloc");
		/* NOTREACHED */
	}
	printMalloc(ptr, MALLOC_SIZE);

	/* shrink, but larger than initial allocation */
	if ((ptr = realloc(ptr, BUFSIZ * 2)) == NULL) {
		err(EXIT_FAILURE, "realloc");
		/* NOTREACHED */
	}
	printMalloc(ptr, BUFSIZ * 4);

	return EXIT_SUCCESS;
}
