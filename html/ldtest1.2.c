/* A second implementation of 'ldtest1'.
 *
 * Build this into a shared library stored in a different location from
 * the first, then use LD_LIBRARY_PATH to switch between 'ldtest'
 * implementations without having to recompile the executable.
 */

#include <stdio.h>

void ldtest0(const char *str) {
	(void)printf("ldtest0 => %s\n", str);
}

void ldtest1(const char *str) {
	(void)printf("alternate ldtest1 implementation => %s\n", str);
}
