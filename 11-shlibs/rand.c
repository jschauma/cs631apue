/* A simple program to introduce the concept of shared libraries.
 *
 * Compiling this program will work, as we have the forward declaration
 * from the header.  Linking the executable will fail,
 * unless we explicitly link against libcrypto.
 *
 * Compare also:
 * cc -Wall rand.c -lcrypto
 * cc -Wall -rdynamic dlopenex.c
 */

#include <openssl/rand.h>

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 16

int
main(int argc, char **argv) {
	int i;
	unsigned char data[NUM];

	if (RAND_bytes(data, NUM) == 0) {
		err(EXIT_FAILURE, "Unable to generate random data: %s\n",
				strerror(errno));
		/* NOTREACHED */
	}

	for (i=0; i<NUM; i++) {
		(void)printf("%02X", data[i]);
	}
	(void)printf("\n");
	exit(EXIT_SUCCESS);
}
