#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NAME "socket"
#define DATA "Just some data"

int main() {
	int child;
	int sock;
	struct sockaddr_un name;
	char buf[BUFSIZ];

	if ((sock = socket(PF_LOCAL, SOCK_DGRAM, 0)) < 0) {
		perror("opening datagram socket");
		exit(EXIT_FAILURE);
	}

	name.sun_family = PF_LOCAL;
	(void)strncpy(name.sun_path, NAME, sizeof(name.sun_path));

	if ((child = fork()) == -1) {
		perror("fork");
	} else if (child) {
		if (bind(sock, (struct sockaddr *)&name, sizeof(struct sockaddr_un))) {
			perror("binding name to datagram socket");
			exit(EXIT_FAILURE);
		}
		if (read(sock, buf, BUFSIZ) < 0) {
			perror("reading from socket");
		}
		(void)printf("Parent (%d) --> reading: %s\n", getpid(), buf);
		(void)close(sock);
	} else {
		printf("Child  (%d) --> sending...\n", getpid());
		if (sendto(sock, DATA, sizeof(DATA), 0, (struct sockaddr *)&name, sizeof(struct sockaddr_un)) < 0) {
			perror("sending datagram message");
		}
		(void)close(sock);
	}

	/* A UNIX domain datagram socket is a 'file'.  If you don't unlink
	 * it, it will remain in the file system. */
	(void)unlink(NAME);
	return EXIT_SUCCESS;
}
