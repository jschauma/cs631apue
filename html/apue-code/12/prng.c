/* A simple program to illustrate the non-randomness
 * of pseudo-random number generators.
 *
 * Invoke multiple times:
 * - once, and all three columns have different numbers
 * - once more, and note the first column remained the
 *   same
 * - twice within the same second (./a.out; echo; ./a.out)
 *   the first two columns will be the same
 *
 * ./a.out; sleep 1; ./a.out; sleep 1; ./a.out; ./a.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int
main() {
	int randNum, srandNum, srandNum2;
	uint32_t arc4r;

	/* This number will be the same for repeated
	 * invocations of this program. */
	randNum = random();

	/* Here we seed the PRNG with the current
	 * time; multiple processes invoked within the
	 * same second will produce the same numbers
	 * here. */
	srandom(time(NULL));
	srandNum = random();

	/* We try to add another element to the seed:
	 * the PID.  Note: it's still possible that
	 * two processes invoked within the same
	 * second can get the same PID.  Know your
	 * randomness requirements! */
	srandom(time(NULL) + getpid());
	srandNum2 = random();

	/* arc4random(3) is the recommended approach
	 * over the obsoleted rand(3) and random(3). */
	arc4r = arc4random();

	printf("%11d\t%11d\t%11d\t%u\n", randNum, srandNum, srandNum2, arc4r);
	exit(EXIT_SUCCESS);
}
