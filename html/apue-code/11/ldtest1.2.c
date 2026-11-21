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
