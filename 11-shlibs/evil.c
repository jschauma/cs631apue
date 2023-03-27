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

/* This file provides an implementation of printf(3), that could be used
 * to do all sorts of nefarious things, while at the same time also
 * implementing printf(3) itself.
 *
 * Build this into a shared library to LD_PRELOAD to illustrate the point.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

int printf(const char *fmt, ...) {
	va_list args;

	fprintf(stdout, "Muahaha, I can do anything now!\n");
	fprintf(stdout, "Including... unlink(whatever)\n");
	fprintf(stdout, "And to have nobody notice, I'll also do what's expected.\n");
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	return strlen(fmt);
}
