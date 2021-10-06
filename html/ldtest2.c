/* A simple dummy function to use in static and dynamic library examples. */

#include <stdio.h>

void ldtest2(const char *str) {
	(void)printf("ldtest2 => %s\n", str);
}
