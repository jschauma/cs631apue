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

/* This program illustrates how zombies are created, and how they
 * disappear again.  If you like, you can try killing some of the zombies.
 */

#include <sys/wait.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main() {
	(void)printf("Let's create some zombies!\n");

	for (int i=0; i<5; i++) {
		pid_t pid;

		if ((pid = fork()) < 0) {
			err(EXIT_FAILURE, "fork error");
			/* NOTREACHED */
		}

		if (pid == 0) {
			/* Do nothing in the child, i.e. immediately exit.  This
			 * creates a zombie until the parent decides to wait for
			 * the child. */
			exit(EXIT_SUCCESS);
		} else {
			(void)printf("====\n");
			system("ps a | grep '[^ ]'a.ou[t]");
			/* We don't wait for our children.  This allows
			 * them to become zombies.  We sleep for a short
			 * time to delay the next iteration of the loop.
			 * When the parent exits, init will reap the zombies. */
			sleep(1);
		}
	}
	(void)printf("I'm going to sleep - try to kill my zombie children, if you like.\n");
	sleep(30);
	(void)printf("That's enough zombies. Let's have init clean them up.\n");
	(void)printf("Remember to run 'ps a | grep a.ou[t]' to verify.\n");
	return EXIT_SUCCESS;
}
