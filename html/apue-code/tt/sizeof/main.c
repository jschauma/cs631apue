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

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv) {
	printf("sizeof(argc): %ld\n", sizeof(argc));
	printf("sizeof(int): %ld\n", sizeof(int));

	printf("sizeof(argv): %ld\n", sizeof(argv));
}
