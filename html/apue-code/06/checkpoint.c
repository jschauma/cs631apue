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

int a;
char *s;
char buf[1024];

int
main(int argc, char **argv) {
	int *b;
	char *string = "abcd";

	*b = 7;
	printf("b at               : 0x%12lX\n", (unsigned long)&b);
	printf("b at               : 0x%12lX\n", (unsigned long)&(*b));


#if 0
	printf("main (function) at : 0x%12lX\n", (unsigned long)&main);
	printf("argc at            : 0x%12lX\n", (unsigned long)&argc);
	printf("argv at            : 0x%12lX\n", (unsigned long)&argv);
	printf("argv[0] at         : 0x%12lX\n", (unsigned long)&argv[0]);
	printf("a at               : 0x%12lX\n", (unsigned long)&a);
	printf("s at               : 0x%12lX\n", (unsigned long)&s);
	printf("buf at             : 0x%12lX\n", (unsigned long)&buf);
	printf("string at          : 0x%12lX\n", (unsigned long)&string);
	printf("fixed string at    : 0x%12lX\n", (unsigned long)&(*string));
#endif

	return 0;
}
