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
 */
int
main(int argc, char * const *argv) {
	int n;
	char buf[BUFSIZ];
	char *secret;

	extern char *optarg;
	extern int optind;

	setprogname(argv[0]);

	if (argc > 1) {
		if ((secret = strdup(argv[1])) == NULL) {
			err(EXIT_FAILURE, "Unable to strdup: ");
			/* NOTREACHED */
		}

		/* This is a poor attempt at concealing the password:
		 * - we're leaking the length of the password (or need
		 *   to manipulate argv itself, which gets complicated
		 *   quickly)
		 */
		//for (n = 0; n < strlen(argv[1]); n++) {
		//	argv[1][n] = '*';
		//}
		/* This would be better, but is actually 'undefined
		 * behaviour', which is why we use 'char * const *'
		 * above. */
		//argv[1] = "********";
		/* Either way, there's a race condition where the pass
		 * is visible in the process table before we even get
		 * here. */
	} else if ((secret = getenv("SECRET")) == NULL) {
		if ((secret = getpass("Password: ")) == NULL) {
			err(EXIT_FAILURE, "Unable to get password from the tty: ");
			/* NOTREACHED */
		}
	}

	printf("Secret: %s\n", secret);

	/* We consume stdin to illustrate the need to
	 * prompt for the password on the tty. */
	while ((n = read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
		/* do stuff with the data */
		;
	}

	exit(EXIT_SUCCESS);
}
