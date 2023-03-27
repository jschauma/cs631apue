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

/* This is a variation of the BSD IPC Tutorial
 * streamreader.  In this variation, we allow the user
 * to specify whether to only listen on IPv4, only on
 * IPv6, or to use a dual-stack socket.
 *
 * ./a.out [4|6]
 */

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 5

/* Everything is stuffed into one giant 'main' here to
 * allow the reader to follow step-by-step without
 * having to jump around.  Normally, you'd probably
 * want to modularize this a bit. */

int
main(int argc, char **argv)
{
	int domain, sock, v4or6;
	void *s;
	socklen_t length, s_size;
	struct sockaddr_storage server;

	if (argc > 2) {
		(void)fprintf(stderr, "Usage: %s [4|6]\n", argv[0]);
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	/* Default is IPv6, but we then become dual
	 * stack by disabling IPV6_ONLY on the socket. */
	v4or6 = 6;

	if (argc == 2) {
		v4or6 = atoi(argv[1]);
		if ((v4or6 != 4) && (v4or6 != 6)) {
			(void)fprintf(stderr, "Usage: %s [4|6]\n", argv[0]);
			exit(EXIT_FAILURE);
			/* NOTREACHED */
		}
	}

	domain = (v4or6 == 4) ? PF_INET : PF_INET6;

	if ((sock = socket(domain, SOCK_STREAM, 0)) < 0) {
		perror("opening stream socket");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	if (domain == PF_INET) {
		struct sockaddr_in *sin = (struct sockaddr_in *)&server;
		memset(sin, 0, sizeof(*sin));

		sin->sin_family = PF_INET;
		sin->sin_addr.s_addr = INADDR_ANY;
		sin->sin_port = 0;
		s = sin;
		s_size = sizeof(*sin);
	} else {
		struct sockaddr_in6 *sin = (struct sockaddr_in6 *)&server;
		memset(sin, 0, sizeof(*sin));

		sin->sin6_family = PF_INET6;
		sin->sin6_addr = in6addr_any;
		sin->sin6_port = 0;
		s = sin;
		s_size = sizeof(*sin);

		/* Neither v4 nor v6 was explicitly
		 * requested, so we do both. */
		if (argc == 1) {
			int off = 0;
			if (setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, (void *)&off, sizeof(off)) < 0) {
				perror("setsockopt");
				exit(EXIT_FAILURE);
				/* NOTREACHED */
			}
		}
	}

	if (bind(sock, (struct sockaddr *)s, s_size) != 0) {
		perror("binding stream socket");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	/* Find out assigned port number and print it out */
	length = sizeof(server);
	if (getsockname(sock, (struct sockaddr *)&server, &length) != 0) {
		perror("getting socket name");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	int num;
	if (domain == PF_INET) {
		struct sockaddr_in *s = (struct sockaddr_in *)&server;
		num = ntohs(s->sin_port);
	} else {
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&server;
		num = ntohs(s->sin6_port);
	}
	(void)printf("Socket has port #%d\n", num);

	if (listen(sock, BACKLOG) < 0) {
		perror("listening");
		exit(EXIT_FAILURE);
		/* NOTREACHED */
	}

	while (1) {
		int fd, rval;
		struct sockaddr_in6 client;
		memset(&client, 0, sizeof(client));

		length = sizeof(client);
		if ((fd = accept(sock, (struct sockaddr *)&client, &length)) < 0) {
			perror("accept");
			continue;
		}

		do {
			char buf[BUFSIZ];
			char claddr[INET6_ADDRSTRLEN];
			struct sockaddr_storage addr;
			socklen_t len;
			const char *rip;
			int port;

			bzero(buf, sizeof(buf));
			if ((rval = read(fd, buf, BUFSIZ)) < 0) {
				perror("reading stream message");
				break;
			}

			if (rval == 0) {
				(void)printf("Ending connection\n");
				break;
			}

			len = sizeof(addr);
			if (getpeername(fd, (struct sockaddr *)&addr, &len) < 0) {
				perror("getpeername");
				break;
			}

			if (domain == PF_INET) {
				struct sockaddr_in *s = (struct sockaddr_in *)&addr;
				port = ntohs(s->sin_port);
				rip = inet_ntop(PF_INET, &s->sin_addr, claddr, sizeof(claddr));
			} else {
				struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
				port = ntohs(s->sin6_port);
				rip = inet_ntop(PF_INET6, &s->sin6_addr, claddr, sizeof(claddr));
			}

			if (rip == NULL) {
				perror("inet_ntop");
				rip = "unknown";
			}
			(void)printf("Client (%s:%d) sent: \"%s\"", rip, port, buf);
		} while (rval != 0);
		(void)close(fd);
	}

	/* NOTREACHED */
}
