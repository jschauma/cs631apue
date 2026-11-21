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

#include <stdlib.h>

struct struct1 {
	char *s;
};

struct struct2 {
	char *s;
	int i;
};

struct struct3 {
	char *s;
	int i;
	int j;
};

struct struct4 {
	struct struct1 s1;
	struct struct2 s2;
	struct struct3 s3;
};

int main() {
	struct struct1 s1;
	struct struct2 s2;
	struct struct3 s3;
	struct struct4 s4;
	exit(EXIT_SUCCESS);
}
