/* This program can be used to illustrate that we can load symbols from
 * dynamic libraries at runtime without actually linking against them.
 *
 * Note that we do _not_ link against libcrypt, nor do we include the
 * unistd.h header.
 *
 * cc -Wall -Werror -Wextra crypt.c
 * cc -Wall -Werror -Wextra -rdynamic dlopenex.c
 */

#include <err.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
printCrypt(const char *s) {
	void *dlhandle;
	char *(*_crypt)(const char *, const char *);
	char *error;

	dlhandle = dlopen("libcrypt.so", RTLD_LAZY);
	if (!dlhandle) {
		err(EXIT_FAILURE, "%s\n", dlerror());
		/* NOTREACHED */
	}

	*(void **) (&_crypt) = dlsym(dlhandle, "crypt");
	if ((error = dlerror()) != NULL)  {
		err(EXIT_FAILURE, "%s\n", error);
		/* NOTREACHED */
	}

	(void)printf("%s\n", _crypt(s, "$1"));
}

int
main(int argc, char **argv) {
	if (argc != 2) {
		(void)fprintf(stderr, "Usage: %s string\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	printCrypt(argv[1]);
	exit(EXIT_SUCCESS);
}
