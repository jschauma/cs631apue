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
