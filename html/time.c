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

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ISO 8601 format is "YYYY-MM-DDTHH:MM:SSZ" */
#define ISO8601_LENGTH 20 + 1 /* +1 for NULL */

int
main() {
	time_t t;
	struct timeval tv;
	struct timespec ts;
	struct tm *tm;

	char buf[ISO8601_LENGTH];

	if ((t = time(NULL)) < 0) {
		err(EXIT_FAILURE, "unable to call time()");
		/* NOTREACHED */
	}
	(void)printf("time(3) says:          %ld\n", t);

	if (gettimeofday(&tv, NULL) < 0) {
		err(EXIT_FAILURE, "unable to gettimeofday()");
		/* NOTREACHED */
	}
	(void)printf("gettimeofday(2) says:  %ld.%d\n", tv.tv_sec, tv.tv_usec);

	if (clock_gettime(CLOCK_REALTIME, &ts) < 0) {
		err(EXIT_FAILURE, "unable to call clock_gettime(2)");
		/* NOTREACHED */
	}
	(void)printf("clock_gettime(2) says: %ld.%ld\n\n", ts.tv_sec, ts.tv_nsec);

	if ((tm = gmtime(&t)) == NULL) {
		err(EXIT_FAILURE, "unable to call gmtime()");
		/* NOTREACHED */
	}
	(void)printf("asctime(3) says:       %s", asctime(tm));

	if ((tm = localtime(&t)) == NULL) {
		err(EXIT_FAILURE, "unable to call gmtime()");
		/* NOTREACHED */
	}
	(void)printf("localtime is:          %s", asctime(tm));

	tm->tm_sec = 0;
	tm->tm_min = 0;
	tm->tm_hour = 0;
	tm->tm_mday = 1;
	tm->tm_mon = 0;
	tm->tm_year = 70;
	tm->tm_isdst = 0;

	if ((t = mktime(tm)) < 0) {
		err(EXIT_FAILURE, "unable to call mktime()");
		/* NOTREACHED */
	}
	(void)printf("epoch date is:         %s", ctime(&t));

	if ((strftime(buf, ISO8601_LENGTH, "%Y-%m-%dT%H:%M:%SZ", tm)) == 0) {
		err(EXIT_FAILURE, "strftime failed");
		/* NOTREACHED */
	}
	(void)printf("ISO8601 formatted:     %s\n", buf);

	return EXIT_SUCCESS;
}
