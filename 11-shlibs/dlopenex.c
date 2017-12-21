/* This program can be used to illustrate that we can load symbols from
 * dynamic libraries at runtime without actually linking against them.
 *
 * Note that we do _not_ link against libcrypto, nor do we include the
 * openssl/rand.h header.
 *
 * cc -Wall -rdynamic dlopenex.c
 */

#include <err.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 16

int
main(int argc, char **argv) {
	int i;
	unsigned char data[NUM];

	void *handle;
	int (*RAND_bytes)(unsigned char *, int);
	char *error;

	handle = dlopen("libcrypto.so", RTLD_LAZY);
	if (!handle) {
		err(EXIT_FAILURE, "%s\n", dlerror());
	}

	*(void **) (&RAND_bytes) = dlsym(handle, "RAND_bytes");
	if ((error = dlerror()) != NULL)  {
		err(EXIT_FAILURE, "%s\n", error);
	}

	if (RAND_bytes(data, NUM) == 0) {
		err(EXIT_FAILURE, "Unable to generate random data: %s\n",
				strerror(errno));
	}

	for (i=0; i<NUM; i++) {
		printf("%02X", data[i]);
	}
	printf("\n");
	exit(EXIT_SUCCESS);
}
