#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>

int
main(void)
{
	if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1 )
		printf("cannot seek\n");
	else
		printf("seek OK\n");
fprintf(stderr, "%s\n", strerror(errno));

	return 0;
}
