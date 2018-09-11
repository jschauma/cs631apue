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
