/* A simple dummy function to use in static and dynamic library examples. */

#include <stdio.h>

void ldtest0(const char *str) {
	(void)printf("ldtest0 => %s\n", str);
}

void ldtest1(const char *str) {
	(void)printf("ldtest1 => %s\n", str);
}
