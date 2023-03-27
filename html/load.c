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

/* Similar to what uptime(1) or w(1) print. */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#define INTERVALS 3

int
main() {
	double loadavg[INTERVALS];

	if (getloadavg(loadavg, INTERVALS) == -1) {
		err(EXIT_FAILURE, "getloadavg");
		/* NOTREACHED */
	}

	(void)printf("%lf %lf %lf\n", loadavg[0], loadavg[1], loadavg[2]);
	return EXIT_SUCCESS;
}
