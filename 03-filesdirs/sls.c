/*
 * simple-ls.c
 * Extremely low-power ls clone.
 * ./simple-ls .
 */

#include <sys/types.h>
#include <ufs/ufs/dir.h>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char **argv) {

	DIR *dp;
	struct dirent *dirp;

	if (argc != 2) {
		fprintf(stderr, "usage: %s dir_name\n", argv[0]);
		exit(1);
	}

	if ((dp = opendir(argv[1])) == NULL ) {
		fprintf(stderr, "can't open '%s'\n", argv[1]);
		exit(1);
	}

	while ((dirp = readdir(dp)) != NULL ) {
		write(STDOUT_FILENO, dirp, sizeof(*dirp));
		//printf("%s: %d %ld\n", dirp->d_name, dirp->d_reclen, UFS_DIRECTSIZ(dirp->d_namlen));
		//printf("%ld\n", sizeof(*dirp));
		//printf("%s: %d %ld\n", dirp->d_name, dirp->d_reclen, UFS_DIRECTSIZ(dirp->d_namlen));
}

	closedir(dp);
	return(0);
}
