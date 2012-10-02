/* This program illustrates how zombies are created, and how they
 * disappear again.  If you like, you can try killing some of the zombies.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char **argv) {
	pid_t pid;
	int i;

	printf("Let's create some zombies!\n");

	for (i=0; i<10; i++) {
		if ((pid = fork()) < 0) {
			fprintf(stderr, "%s: fork error: %s\n",
					argv[0], strerror(errno));
			exit(1);
		}

		if (pid == 0) {
			/* Do nothing in the child, ie immediately exit.  This
			 * creates a zombie until the parent decides to wait for
			 * the child. */
			exit(0);
		} else {
			printf("====\n");
			system("ps a | grep '[^ ]'a.ou[t]");
			/* We don't wait for our children.  This allows
			 * them to become zombies.  We sleep for a short
			 * time to delay the next iteration of the loop.
			 * When the parent exits, init will reap the zombies. */
			sleep(2);
			/* We wait for every other child to show that
			 * zombies are reaped eventually if the parent
			 * waits. */
			if (i%2)
				wait(NULL);
		}
	}
	printf("That's enough zombies. Let's have init clean them up.\n");
	printf("Remember to run 'ps a | grep a.ou[t]' to verify.\n");
	exit(0);
}
