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
	(void)argc;
	(void)argv;

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
