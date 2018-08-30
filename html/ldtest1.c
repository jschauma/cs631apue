/* A simple dummy function to use in static and dynamic library examples. */

#include <stdio.h>

void ldtest1(const char *str) {
	printf("ldtest1 => %s\n", str);
}
