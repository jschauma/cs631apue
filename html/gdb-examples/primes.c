#include <math.h>

#include "defs.h"

void
checkprime(int k)
{
	int j;

	/* here's what we want to do:
	 * see if j divides k, for all values j which are
	 * (a) themselves prime (no need to try j if it is nonprime), and
	 * (b) less than or equal to sqrt(k) (if k has a divisor larger
	 *    than this square root, it must also have a smaller one,
	 *    so no need to check for larger ones) */

	j = 2;

	while(j++) {
		if (primes[j] == 1)
			if (k % j == 0) {
				primes[k] = 0;
				return;
			}
	}

	/* if we get here, then there were no divisors of K, so it is prime */
	primes[k] = 1;
}
