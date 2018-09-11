#include <stdio.h>

#include "greet.h"

int main(void) {
	greet();
	if (setgreeting("Howdy!") != 0) {
		fprintf(stderr, "Unable to set greeting!\n");
	}
	greet();
	hello("world", getgreeting());
	return 0;
}
