/* A simple program to illustrate the options of
 * getting a secret from a user.  Note that we are
 * reading input from stdin, so we can't juse prompt
 * the user for the password there.  Fortunately,
 * getpass(3) does the right thing already.
 *
 * Try it as:
 * - echo foo | ./a.out
 * - echo foo | env SECRET=moo ./a.out
 * - ./a.out password
 *
 * Use the debugger to inspect the secret in memory
 * after we change where 'secret' points to and / or
 * inspect the core dump after we called abort(3):
 * $ echo | ./a.out asdasd
 * $ strings a.out.core | grep asdasd
 *
 * Note also that the stdout buffer retains the
 * secret, as well as getpass(3) internally;
 * getpass_r(3) can help so you can zero out the
 * password, but to avoid the stdout buffer, you'd
 * need to use unbuffered I/O.
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/* Note: we are using 'char * const *argv' here, to be explicit
 * about which parts of argv[][] we can monkey around with.
 * Without this, the compiler may let you get away
 * with changing e.g. argv[i], but that may actually
 * fail depending on the platform/compiler.  See also:
 * https://bytes.com/topic/c/answers/435791-argv-array-modifiable
 * https://stevens.netmeister.org/631/apue-code/12/const.c
 */
int
main(int argc, char * const *argv) {
	int n;
	int mustFree = 0;
	char buf[BUFSIZ];
	char *secret;

	if (argc > 1) {
		if ((secret = strdup(argv[1])) == NULL) {
			err(EXIT_FAILURE, "Unable to strdup: ");
			/* NOTREACHED */
		}
		mustFree = 1;

		/* This is a poor attempt at concealing the password:
		 * - we're leaking the length of the password (or need
		 *   to manipulate argv itself, which gets complicated
		 *   quickly)
		 */
		for (n = 0; n < (int)strlen(argv[1]); n++) {
			argv[1][n] = '*';
		}
		/* This would be better, but is actually 'undefined
		 * behaviour', which is why we use 'char * const *'
		 * above. */
		//argv[1] = "********";
		/* Either way, there's a race condition where the secret
		 * is visible in the process table before we even get
		 * here. */
	} else if ((secret = getenv("SECRET")) == NULL) {

		/* The getpass() function leaves its result in an
		 * internal static object. Alternatively, use
		 * getpass_r(3). */
		if ((secret = getpass("Password: ")) == NULL) {
			err(EXIT_FAILURE, "Unable to get password from the tty: ");
			/* NOTREACHED */
		}
		/* If you use getpass_r(3), remember
		 * to free(3)! */
		//mustFree = 1;
	}

	(void)printf("Secret: %s\n", secret);
	/* printf(3) will leave the secret in the buffer!
	 * Avoid by using unbuffered I/O: */
	/*
	write(STDOUT_FILENO, "Secret: ", strlen("Secret: "));
	write(STDOUT_FILENO, secret, strlen(secret));
	write(STDOUT_FILENO, "\n", 1);
	*/

	/* We consume stdin to illustrate the need to
	 * prompt for the password on the tty. */
	while ((n = read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
		/* do stuff with the data */
		;
	}

	/* Insufficient! We are just changing what
	 * 'secret' points to -- the actual secret
	 * remains in memory at the previous location.
	 * (Use the debugger to verify.) */
	secret = "not a secret";

	/* Instead: remove the above statement and
	 * then explicitly zero out the secret in
	 * memory.  Make sure to zero out _all_ bytes. */
	//memset(secret, 0, strlen(secret));

	/* We intentionally abort here to cause a core
	 * dump.  Inspect the core dump (e.g., using
	 * strings(1)) to find your secret. */
	abort();

	if (mustFree) {
		free(secret);
	}
	exit(EXIT_SUCCESS);
}
