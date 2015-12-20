/*
 * simple-ls.c
 * Extremely low-power ls clone.
 * ./simple-ls .
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
printType(const struct stat sb) {
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
}

int
main(int argc, char **argv) {

	DIR *dp;
	struct dirent *dirp;

	if (argc != 2) {
		fprintf(stderr, "usage: %s dir_name\n", argv[0]);
		exit(1);
	}

	if ((dp = opendir(argv[1])) == NULL) {
		fprintf(stderr, "can't open '%s'\n", argv[1]);
		exit(1);
	}

	if (chdir(argv[1]) == -1) {
		fprintf(stderr, "can't chdir to '%s': %s\n", argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}

	while ((dirp = readdir(dp)) != NULL) {
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

		printf("%s (", dirp->d_name);
		printType(sb);
		printf(" - ");

		if (lstat(dirp->d_name, &sb) == -1) {
			fprintf(stderr,"Can't stat %s: %s\n", dirp->d_name,
						strerror(errno));
			continue;
		}
		printType(sb);
		printf(")\n");
	}

	closedir(dp);
	return(0);
}
