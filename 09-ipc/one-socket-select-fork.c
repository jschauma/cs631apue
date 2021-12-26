/* This program shows how we can combine select(2) and
 * a per-connection fork(2) model to handle
 * simultaneous connections in a typical server setup.
 */
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>

#include <netinet/in.h>

#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 5

#ifndef SLEEP
#define SLEEP   5
#endif

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
handleConnection(int fd, struct sockaddr_in6 client)
{
	const char *rip;
	int rval;
	char claddr[INET6_ADDRSTRLEN];

	if ((rip = inet_ntop(PF_INET6, &(client.sin6_addr), claddr, INET6_ADDRSTRLEN)) == NULL) {
		perror("inet_ntop");
		rip = "unknown";
	} else {
		(void)printf("Client connection from %s!\n", rip);
	}

	do {
		char buf[BUFSIZ];
		bzero(buf, sizeof(buf));
		if ((rval = read(fd, buf, BUFSIZ)) < 0)
			perror("reading stream message");
		else if (rval == 0)
			printf("Ending connection from %s.\n", rip);
		else
			printf("Client (%s) sent: %s", rip, buf);
	} while (rval != 0);
	(void)close(fd);
	exit(EXIT_SUCCESS);
	/* NOTREACHED */
}
void
handleSocket(int s)
{
	int fd;
	pid_t pid;
	struct sockaddr_in6 client;
	socklen_t length;

	memset(&client, 0, sizeof(client));

	length = sizeof(client);
	if ((fd = accept(s, (struct sockaddr *)&client, &length)) < 0) {
		perror("accept");
		return;
	}

	if ((pid = fork()) < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	} else if (!pid) {
		handleConnection(fd, client);
		/* NOTREACHED */
	}
	/* parent silently returns */
}

void
reap() {
	wait(NULL);
}

int
main()
{
	int s1;

	if (signal(SIGCHLD, reap) == SIG_ERR) {
		perror("signal");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	s1 = createSocket();

	for (;;) {
		fd_set ready;
		struct timeval to;

		FD_ZERO(&ready);
		FD_SET(s1, &ready);
		to.tv_sec = SLEEP;
		to.tv_usec = 0;
		if (select(s1 + 1, &ready, 0, 0, &to) < 0) {
			if (errno != EINTR) {
				perror("select");
			}
			continue;
		}
		if (FD_ISSET(s1, &ready)) {
			handleSocket(s1);
		} else {
			(void)printf("Idly sitting here, waiting for connections...\n");
		}
	}

	/* NOTREACHED */
}
