#include "API_server.h"
/* netinet/in.h also includes sys/types.h and sys/socket.h */
/* sys/types.h needed for socket.h portability beyond Linux / POSIX.1-2001 */
/* sockaddr_in htons htonl; sys/socket.h: socket bind listen accept recv */
#include <netinet/in.h>
/* inet_ntoa */
#include <arpa/inet.h>
/* perror printf setbuf */
#include <stdio.h>
/* close */
#include <unistd.h>
/* exit EXIT_FAILURE EXIT_SUCCESS */
#include <stdlib.h>
/* sigaction */
#include <signal.h>
/* memset */
#include <string.h>


/* socket fds made global to be seen by signal handler */
/* declaration in API_server.h */
/* -1 represents "unused" state */
int server_fd = -1;
int client_fd = -1;

/* global head of list for storage in process memory instead of file or DB */
/* declaration in API_server.h */
todo_t *todos = NULL;


/**
 * errorExit - exits calling function with message, after closing any open
 *   sockets
 *
 * @error_msg: message to print on exit
 */
void errorExit(char *error_msg)
{
	/* tries to complete transmissions first, otherwise use shutdown(2) */
	if (server_fd != -1)
	{
		if (close(server_fd) != 0)
		{
			perror("errorExit: close");
			exit(EXIT_FAILURE);
		}
		server_fd = -1;
	}

	if (client_fd != -1)
	{
		if (close(client_fd) != 0)
		{
			perror("errorExit: close");
			exit(EXIT_FAILURE);
		}
		client_fd = -1;
	}

#if SRC_VERSION >= 8
	freeTodos();
#endif

	if (error_msg)
		perror(error_msg);

	exit(EXIT_FAILURE);
}


/**
 * listenTCPIPv4Socket - opens a new IPv4/TCP socket, binds to a given port,
 *   and starts listening on that socket
 *
 * @port: port number to assign to socket
 * Return: fd/socket id of new IPv4/TCP socket (direct exit on failure)
 */
void listenTCPIPv4Socket(uint16_t port)
{
	struct sockaddr_in addr = {0};

	server_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		errorExit("listenTCPIPv4Socket: socket");

	/* address family in host byte order */
	addr.sin_family = AF_INET;
	/* port number converted to network byte order (MSB) */
	addr.sin_port = htons(port);
	/* IPv4 address converted to network byte order (MSB) */
	/* INADDR_ANY is wildcard for IPv4, see getaddrinfo(3) */
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_fd, (struct sockaddr *)&addr,
		 sizeof(struct sockaddr_in)) == -1)
		errorExit("listenTCPIPv4Socket: bind");

	if (listen(server_fd, MAX_PENDING) == -1)
		errorExit("listenTCPIPv4Socket: listen");

	/* assuming LSB environment (network byte order is canonically MSB) */
	printf("Server listening on port %i\n",
	       __builtin_bswap16((uint16_t)(addr.sin_port)));
}


/**
 * API_server - opens an IPv4/TCP socket, and listens to traffic on port
 *   8080 (Any address).
 *
 * Waits for the first incoming connection, prints the IP address of the
 *   connected client, waits for an incoming message from the connected client,
 *   prints the received message, closes the connection with the client
 *
 * Return: EXIT_FAILURE or EXIT_SUCCESS
 */
int API_server(void)
{
	struct sockaddr_in client_addr = {0};
	socklen_t client_addr_sz = sizeof(struct sockaddr_in);
	char recv_buf[RECV_BUFSZ] = {0};
	HTTP_request_t *request = NULL;

	listenTCPIPv4Socket(8080);
	for (;;)
	{
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
				   &client_addr_sz);
		if (client_fd == -1)
		{
			if (errno == EINTR)
				errorExit(NULL);
			errorExit("API_server: accept");
		}
#if SRC_VERSION <= 7
		printf("Client connected: %s\n",
		       inet_ntoa(client_addr.sin_addr));
#else
		printf("%s ", inet_ntoa(client_addr.sin_addr));
#endif
		if (recv(client_fd, (void *)recv_buf, RECV_BUFSZ, 0) == -1)
		{
			HTTP_response(500, NULL, NULL);
			errorExit("API_server: recv");
		}
		request = parseRequest(recv_buf);
		if (request)
		{
#if SRC_VERSION <= 7
			printRequest(request);
#else
			runMethod(request);
#endif
			freeRequest(request);
		}
		memset(recv_buf, 0, RECV_BUFSZ);
	}
	return (EXIT_SUCCESS);
}


/**
 * main - entry point for API_server
 *
 * Return: return value of API_server
 */
int main(void)
{
	setSigHandler();

	/* make all output unbuffered instead of repeated calls to fflush() */
	setbuf(stdout, NULL);

	return (API_server());
}
