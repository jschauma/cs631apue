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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 12345
#define DATA "Just some data"

int main() {
	int child;
	int sock;
	struct sockaddr_in name;
	char buf[BUFSIZ];

	memset(&name, 0, sizeof(name));

	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("opening datagram socket");
		exit(EXIT_FAILURE);
	}

	name.sin_family = PF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = htons(PORT);

	if ((child = fork()) == -1) {
		perror("fork");
	} else if (child) {
		if (bind(sock, (struct sockaddr *)&name, sizeof(name))) {
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
		if (sendto(sock, DATA, sizeof(DATA), 0, (struct sockaddr *)&name, sizeof(name)) < 0) {
			perror("sending datagram message");
		}
		(void)close(sock);
	}
	return EXIT_SUCCESS;
}
