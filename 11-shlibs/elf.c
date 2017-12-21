/* A simple program to introduce the concept of shared libraries.
 *
 * Compiling this program will work, as we have the forward declaration
 * from the libelf header.  Linking the executable will fail,
 * unless we explicitly link against libelf.
 *
 * Compare 'cc -Wall math.c -lelf' with 'cc -Wall -rdynamic dlopenex.c
 * -dl'.
 */

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <libelf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char **argv) {
	int fd;
	Elf *e;
	char *id;

	if (argc != 2) {
		err(EXIT_FAILURE, "Usage: %s string\n", argv[0]);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		err(EXIT_FAILURE, "Unable to open %s: %s\n",
				argv[1], strerror(errno));
	}

	if (elf_version(EV_CURRENT) == EV_NONE) {
		err(EXIT_FAILURE, "ELF library too old");
	}

	if ((e = elf_begin(fd, ELF_C_READ, (Elf *) 0)) == NULL) {
		err(EXIT_FAILURE, "Unable to elf_begin %s: %s\n",
				argv[1], strerror(elf_errno()));
	}

	if ((id = elf_getident(e, 0)) == NULL) {
		err(EXIT_FAILURE, "Unable to elf_getident: %s\n",
				strerror(elf_errno()));
	}

	(void)close(fd);
	printf("%s\n", id);
	exit(EXIT_SUCCESS);
}
