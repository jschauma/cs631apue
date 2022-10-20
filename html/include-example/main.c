#include <stdlib.h>

#include "print.h"

int
main(int argc, char **argv) {
	struct foo F;

	F.a = 0;
	F.b = 1;
	printFoo(F);

	return EXIT_SUCCESS;
}
