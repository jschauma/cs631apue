#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "greet.h"

char *libgreet_greeting = "Hello!";

void greet(void) {
	printf("%s\n", libgreet_greeting);
}

void hello(const char *friend, const char *greeting) {
	printf("%s: %s\n", friend, greeting);
}

char * getgreeting(void) {
	char *tmp;

	if ((tmp = strdup(libgreet_greeting)) == NULL) {
		fprintf(stderr, "getgreeting: Unable to strdup: %s\n",
				strerror(errno));
		return NULL;
	}

	return tmp;
}

int setgreeting(const char *greeting) {
	char *tmp;

	if ((tmp = strdup(greeting)) == NULL) {
		fprintf(stderr, "setgreeting: Unable to strdup: %s\n",
				strerror(errno));
		return -1;
	}
	libgreet_greeting = tmp;
	return 0;
}
