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
