#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	char *buf;
	buf = malloc(1<<9);
	fgets(buf, 1024, stdin);
	printf("%s\n", buf);
	return 1;
}
