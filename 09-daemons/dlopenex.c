/* This program can be used to illustrate that we can load symbols from
 * dynamic libraries at runtime without actually linking against them.
 *
 * Since (some) Linux versions don't use getprogname(3)/setprogname(3), we
 * extract these function definitions from the BSD compatibility library, but
 * unlike in the 'setget' example, we are _not_ linking against the library.
 *
 * Compile with 'cc -Wall -rdynamic dlopenex.c -dl'; compare with 'cc
 * -Wall setget.c -lbsd'.
 */

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv) {
	void *handle;
	void (*setprogname)(const char *);
	char *(*getprogname)(void);
	char *error;

	handle = dlopen("libbsd.so", RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "%s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	(void)dlerror();

	*(void **) (&setprogname) = dlsym(handle, "setprogname");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		exit(EXIT_FAILURE);
	}

	*(void **) (&getprogname) = dlsym(handle, "getprogname");
	if ((error = dlerror()) != NULL)  {
		fprintf(stderr, "%s\n", error);
		exit(EXIT_FAILURE);
	}


	setprogname("dlopenex");
	printf("My name is '%s'.\n", getprogname());
	dlclose(handle);
	exit(EXIT_SUCCESS);
}
