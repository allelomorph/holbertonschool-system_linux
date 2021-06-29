#include "API_server.h"
/* netinet/in.h also includes sys/types.h and sys/socket.h */
/* sys/types.h needed for socket.h portability beyond Linux / POSIX.1-2001 */
/* sockaddr_in htons htonl; sys/socket.h: socket bind listen accept recv */
#include <netinet/in.h>
/* inet_ntoa */
#include <arpa/inet.h>
/* perror printf */
#include <stdio.h>
#include <errno.h>
/* close */
#include <unistd.h>
/* exit EXIT_FAILURE EXIT_SUCCESS */
#include <stdlib.h>
/* sigaction */
#include <signal.h>
/* memset */
#include <string.h>


/* socket fds made global to be seen by signal handler */
/* -1 represents "unused" state */
int server_fd = -1;
int client_fd = -1;

#define MAX_PENDING 10
#define RECV_BUFSZ 1024

/**
 * SIGINT_SIGTERM_handler - closes any open socket fds in the event of a
 *   termination or interrupt
 *
 * @signum: code number of incoming signal
 */
void SIGINT_SIGTERM_handler(int signum)
{
	/* signum required by expected handler prototype */
	(void)signum;

	/* tries to complete transmissions first, otherwise use shutdown(2) */
	if (server_fd != -1)
	{
		if (close(server_fd) != 0)
		{
			perror("SIGINT_SIGTERM_handler: close");
			exit(EXIT_FAILURE);
		}
		server_fd = -1;
	}

	if (client_fd != -1)
	{
		if (close(client_fd) != 0)
		{
			perror("SIGINT_SIGTERM_handler: close");
			exit(EXIT_FAILURE);
		}
		client_fd = -1;
	}
}


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

	if (error_msg)
		perror(error_msg);

	exit(EXIT_FAILURE);
}


/**
 * newTCPIPv4Socket - opens a new IPv4/TCP socket
 *
 * @port: port number to assign to socket
 * @addr: pointer to struct to contain address info for new socket
 * Return: fd/socket id of new IPv4/TCP socket (direct exit on failure)
 */
int newTCPIPv4Socket(uint16_t port, struct sockaddr_in *addr)
{
	int sock_fd;

	if (!addr)
	{
		fprintf(stderr, "API_server: newTCPIPv4Socket: NULL addr\n");
		exit(EXIT_FAILURE);
	}

	sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1)
	{
		perror("API_server: newTCPIPv4Socket: socket");
		exit(EXIT_FAILURE);
	}

	/* address family in host byte order */
	addr->sin_family = AF_INET;
	/* port number converted to network byte order (MSB) */
	addr->sin_port = htons(port);
	/* IPv4 address converted to network byte order (MSB) */
	/* INADDR_ANY is wildcard for IPv4, see getaddrinfo(3) */
	addr->sin_addr.s_addr = htonl(INADDR_ANY);

	return (sock_fd);
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
	struct sockaddr_in server_addr = {0}, client_addr = {0};
	socklen_t client_addr_sz = sizeof(struct sockaddr_in);
	char recv_buf[RECV_BUFSZ] = {'\0'};
	struct sigaction sa;

	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = SIGINT_SIGTERM_handler;
        if (sigaction(SIGINT, &sa, NULL) == -1 ||
	    sigaction(SIGTERM, &sa, NULL) == -1)
		errorExit("API_server: sigaction");

	server_fd = newTCPIPv4Socket(8080, &server_addr);
	if (bind(server_fd, (struct sockaddr *)&server_addr,
		 sizeof(struct sockaddr_in)) == -1)
		errorExit("API_server: bind");

	if (listen(server_fd, MAX_PENDING) == -1)
		errorExit("API_server: listen");

	/* assuming LSB environment (network byte order is canonically MSB) */
	printf("Server listening on port %i\n",
	       __builtin_bswap16((uint16_t)(server_addr.sin_port)));

	for (;;)
	{
		client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
				   &client_addr_sz);
		if (client_fd == -1)
			errorExit("API_server: accept");
		printf("Client connected: %s\n",
		       inet_ntoa(client_addr.sin_addr));

		if (recv(client_fd, (void *)recv_buf, RECV_BUFSZ, 0) == -1)
			errorExit("API_server: recv");
		printf("Raw request:\"%s\"\n", recv_buf);

		parseHTTPRequest(recv_buf);
		HTTP_response(200, NULL, NULL);

		if (close(client_fd) != 0)
			errorExit("API_server: close");
		client_fd = -1;
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
	return (API_server());
}
