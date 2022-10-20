#include <stdio.h>

#include "print.h"

void
printFoo(const struct foo F) {
	(void)printf("F.a: %d\nF.b: %d\n", F.a, F.b);
}
