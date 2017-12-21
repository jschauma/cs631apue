/* A simple program to illustrate the non-randomness
 * of pseudo-random number generators.
 *
 * Invoke multiple times:
 * - once, and all three columns have different numbers
 * - once more, and note the first column remained the
 *   same
 * - twice within the same second (./a.out; echo; ./a.out)
 *   the first two columns will be the same
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int
main(int argc, char **argv) {
	int randNum, srandNum, srandNum2;

	/* This number will be the same for repeated
	 * invocations of this program. */
	randNum = rand();

	/* Here we seed the PRNG with the current
	 * time; multiple processes invoked within the
	 * same second will produce the same numbers
	 * here. */
	srand(time(NULL));
	srandNum = rand();

	/* We try to add another element to the seed:
	 * the PID.  Note: it's still possible that
	 * two processes invoked within the same
	 * second can get the same PID.  Know your
	 * randomness requirements! */
	srand(time(NULL) + getpid());
	srandNum2 = rand();

	printf("%11d\t%11d\t%11d\n", randNum, srandNum, srandNum2);
	exit(EXIT_SUCCESS);
}
