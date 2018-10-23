#! /bin/sh

set -e

mkdir -p testdir
cd testdir

touch empty
mkfifo fifo
echo file > file
ln file file2
ln -s /nowhere
ln -s file2 symlink
touch -a -t 201810012222 atime
touch -t 196912010000 longago
touch -t 300011111111 inthefuture
mkdir subdir
chmod 1755 subdir
ln -s . subdir/loopdir
touch executable
chmod 755 executable
touch suid:sgid
chmod 6755 suid:sgid
touch missing-exec
chmod 4000 missing-exec
touch " "
touch "	"
touch "foobar"

su root -c "cp /dev/wd0 ."
touch noowner
su root -c "chown 1234 noowner"
touch nogroup
su root -c "chown :1234 nogroup"
ln -s / "ls-LR"
su root -c "cp -a /dev/null null"
touch $(yes a | head -255 | tr -d '\n')

cat >hole.c <<EOF
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

#define BIGNUM 10240000

int
main(void) {
	int fd;

	if ((fd = creat("file.hole", S_IRUSR | S_IWUSR)) < 0) {
		perror("creat error");
		return EXIT_FAILURE;
	}
	if (write(fd, buf1, strlen(buf1)) != (ssize_t)strlen(buf1)) {
		perror("error writing buf1");
		return EXIT_FAILURE;
	}

	if (lseek(fd, BIGNUM, SEEK_CUR) == -1) {
		perror("lseek error");
		return EXIT_FAILURE;
	}

	if (write(fd, buf2, strlen(buf2)) != (ssize_t)strlen(buf2)) {
		perror("error writing buf2");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
EOF

cc -Wall -Werror hole.c
./a.out
