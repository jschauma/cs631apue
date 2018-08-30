#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int lock;

int main()
{
	lock = 0;
	char *s;
	if (fork())
		s = "parent";
	else
		s = "child";

	for (;;) {
		if (lock) {
			sleep(5);
			printf("%s sleeping...\n", s);
		}
		else {
			lock++;
			printf("%s: %d: %d\n", s, getpid(), lock);
			lock--;
			sleep(2);
		}
	}
	return 0;
}
