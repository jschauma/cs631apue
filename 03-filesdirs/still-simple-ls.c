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

/* This still very simple ls clone illustrates the use of the stat(2)
 * family of functions to print out some information about the files in
 * the given directory.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
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
		exit(EXIT_FAILURE);
	}

	if ((dp = opendir(argv[1])) == NULL ) {
		fprintf(stderr, "can't open '%s': %s\n", argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (chdir(argv[1]) == -1) {
		fprintf(stderr, "can't chdir to '%s': %s\n", argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}

	while ((dirp = readdir(dp)) != NULL ) {
		struct stat sb;
		if (stat(dirp->d_name, &sb) == -1) {
			fprintf(stderr, "Can't stat %s: %s\n", dirp->d_name,
						strerror(errno));

			if (lstat(dirp->d_name, &sb) == -1) {
				fprintf(stderr,"Can't stat %s: %s\n", dirp->d_name,
						strerror(errno));
				continue;
			}
		}

		printf("%s: ", dirp->d_name);
		if (S_ISREG(sb.st_mode))
			printf("regular file");
		else if (S_ISDIR(sb.st_mode))
			printf("directory");
		else if (S_ISCHR(sb.st_mode))
			printf("character special");
		else if (S_ISBLK(sb.st_mode))
			printf("block special");
		else if (S_ISFIFO(sb.st_mode))
			printf("FIFO");
		else if (S_ISLNK(sb.st_mode))
			printf("symbolic link");
		else if (S_ISSOCK(sb.st_mode))
			printf("socket");
		else
			printf("unknown");

		printf(" -- according to stat\n");

		if (lstat(dirp->d_name, &sb) == -1) {
			fprintf(stderr,"Can't stat %s: %s\n", dirp->d_name,
						strerror(errno));
			continue;
		}

		printf("%s: ", dirp->d_name);
		if (S_ISREG(sb.st_mode))
			printf("regular file");
		else if (S_ISDIR(sb.st_mode))
			printf("directory");
		else if (S_ISCHR(sb.st_mode))
			printf("character special");
		else if (S_ISBLK(sb.st_mode))
			printf("block special");
		else if (S_ISFIFO(sb.st_mode))
			printf("FIFO");
		else if (S_ISLNK(sb.st_mode))
			printf("symbolic link");
		else if (S_ISSOCK(sb.st_mode))
			printf("socket");
		else
			printf("unknown");

		printf(" -- according to lstat\n\n");
	}

	closedir(dp);
	exit(EXIT_SUCCESS);
}
