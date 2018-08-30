#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

int primes[MAXPRIMES];
int upperb;

int
main(int argc, char **argv)
{
	int n;

	printf("Enter upper bound: ");
	scanf("%d", &upperb);

	primes[2] = 1;

	for (n=3; n<=upperb; n+=2) {
		checkprime(n);
		if (primes[n])
			printf("%d is a prime\n", n);
	}

	return 0;
}
