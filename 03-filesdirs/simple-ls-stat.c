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
 * Variant of 'simple-ls' that illustrates the
 * difference between stat(2) and lstat(2).
 *
 * Create a few files and symlink to different types
 * of files to show.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *
getType(const struct stat sb) {
	if (S_ISREG(sb.st_mode))
		return "regular file";
	else if (S_ISDIR(sb.st_mode))
		return "directory";
	else if (S_ISCHR(sb.st_mode))
		return "character special";
	else if (S_ISBLK(sb.st_mode))
		return "block special";
	else if (S_ISFIFO(sb.st_mode))
		return "FIFO";
	else if (S_ISLNK(sb.st_mode))
		return "symbolic link";
	else if (S_ISSOCK(sb.st_mode))
		return "socket";
	else
		return "unknown";
}

int
main(int argc, char **argv) {

	DIR *dp;
	struct dirent *dirp;

	if (argc != 2) {
		fprintf(stderr, "usage: %s dir_name\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((dp = opendir(argv[1])) == NULL) {
		fprintf(stderr, "can't open '%s'\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (chdir(argv[1]) == -1) {
		fprintf(stderr, "can't chdir to '%s': %s\n", argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}

	while ((dirp = readdir(dp)) != NULL) {
		struct stat sb;
		char *statType;

		printf("%s: ", dirp->d_name);
		if (stat(dirp->d_name, &sb) == -1) {
			fprintf(stderr, "Can't stat %s: %s\n", dirp->d_name,
						strerror(errno));
			statType = "unknown";
		} else {
			statType = getType(sb);
		}

		if (lstat(dirp->d_name, &sb) == -1) {
			fprintf(stderr,"Can't lstat %s: %s\n", dirp->d_name,
						strerror(errno));
			continue;
		} else if (S_ISLNK(sb.st_mode)) {
			printf("symlink to ");
		}
		printf("%s\n", statType);
	}

	(void)closedir(dp);
	return EXIT_SUCCESS;
}
