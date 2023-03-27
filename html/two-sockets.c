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
 * This program illustrates the dilemma of handling
 * multiple file descriptors.  Compare with
 * two-sockets-select.c to show how I/O multiplexing
 * could be done.
 */

#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>

#include <netinet/in.h>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 5

int
createSocket(void)
{
	int sock;
	socklen_t length;
	struct sockaddr_in6 server;

	memset(&server, 0, sizeof(server));

	if ((sock = socket(PF_INET6, SOCK_STREAM, 0)) < 0) {
		perror("opening stream socket");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	server.sin6_family = PF_INET6;
	server.sin6_addr = in6addr_any;
	server.sin6_port = 0;
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) != 0) {
		perror("binding stream socket");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	length = sizeof(server);
	if (getsockname(sock, (struct sockaddr *)&server, &length) != 0) {
		perror("getting socket name");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}
	(void)printf("Socket has port #%d\n", ntohs(server.sin6_port));

	if (listen(sock, BACKLOG) < 0) {
		perror("listening");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	return sock;
}

void
handleSocket(int s)
{
	int fd, rval;
	char claddr[INET6_ADDRSTRLEN];
	struct sockaddr_in6 client;
	socklen_t length;

	length = sizeof(client);
	memset(&client, 0, length);

	if ((fd = accept(s, (struct sockaddr *)&client, &length)) < 0) {
		perror("accept");
		return;
	}

	do {
		char buf[BUFSIZ];
		bzero(buf, sizeof(buf));
		if ((rval = read(fd, buf, BUFSIZ)) < 0) {
			perror("reading stream message");
		}

		if (rval == 0) {
			(void)printf("Ending connection\n");
		} else {
			const char *rip;
			if ((rip = inet_ntop(PF_INET6, &(client.sin6_addr), claddr, INET6_ADDRSTRLEN)) == NULL) {
				perror("inet_ntop");
				rip = "unknown";
			} else {
				(void)printf("Client (%s) sent: %s\n", rip, buf);
			}
		}
	} while (rval != 0);
	(void)close(fd);
}

/*
 * This program shows that the second socket is blocked until we
 * handled socket number one.1
 */
int
main()
{
	int s1, s2;

	s1 = createSocket();
	s2 = createSocket();

	for (;;) {
		handleSocket(s1);
		handleSocket(s2);
	}

	/* NOTREACHED */
}
