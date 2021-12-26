#include <err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA2 "Hello, I'm buf2!"
#define DATA3 "Hello, I'm buf3!"

void
printBufs(long n) {
	char *buf, *buf2, *buf3;

	if ((buf = malloc(n)) == NULL) {
		err(EXIT_FAILURE, "malloc");
	}
	if ((buf2 = malloc(8)) == NULL) {
		err(EXIT_FAILURE, "malloc");
	}
	if ((buf3 = malloc(8)) == NULL) {
		err(EXIT_FAILURE, "malloc");
	}

	(void)strncpy(buf2, DATA2, 8);
	buf2[8-1] = '\0';
	(void)strncpy(buf3, DATA3, 8);
	buf3[8-1] = '\0';
	(void)fgets(buf, n, stdin);

	printf("buf is : '%s'\n", buf);
	printf("buf2 is: '%s'\n", buf2);
	printf("buf3 is: '%s'\n", buf3);
}
