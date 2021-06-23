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


#define MAX_PENDING 10
#define RECV_BUFSZ 1024


/**
 * errorExit - exits calling function with message, after closing the socket
 *   specified by fd sckt_id
 *
 * @sock_id: fd of socket to close on exit
 * @error_msg: message to print on exit
 */
void errorExit(int sock_id, char *error_msg)
{
	/* tries to complete transmissions first, otherwise use shutdown(2) */
	if (close(sock_id) != 0)
	{
		perror("simpleServer: close");
		exit(EXIT_FAILURE);
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
	int sock_id;

	if (!addr)
	{
		fprintf(stderr, "simpleServer: newTCPIPv4Socket: NULL addr\n");
		exit(EXIT_FAILURE);
	}

	sock_id = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_id == -1)
	{
		perror("simpleServer: newTCPIPv4Socket: socket");
		exit(EXIT_FAILURE);
	}

	/* address family in host byte order */
	addr->sin_family = AF_INET;
	/* port number converted to network byte order (MSB) */
	addr->sin_port = htons(port);
	/* IPv4 address converted to network byte order (MSB) */
	/* INADDR_ANY is wildcard for IPv4, see getaddrinfo(3) */
	addr->sin_addr.s_addr = htonl(INADDR_ANY);

	return (sock_id);
}


/**
 * simpleServer - opens an IPv4/TCP socket, and listens to traffic on port
 *   12345 (Any address).
 *
 * Waits for the first incoming connection, prints the IP address of the
 *   connected client, waits for an incoming message from the connected client,
 *   prints the received message, closes the connection with the client
 *
 * Return: EXIT_FAILURE or EXIT_SUCCESS
 */
int simpleServer(void)
{
	int server_id, client_id;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_sz;
	char recv_buf[RECV_BUFSZ];

	server_id = newTCPIPv4Socket(12345, &server_addr);
	if (bind(server_id, (struct sockaddr *)&server_addr,
		 sizeof(struct sockaddr_in)) == -1)
		errorExit(server_id, "simpleServer: bind");

	if (listen(server_id, MAX_PENDING) == -1)
		errorExit(server_id, "simpleServer: listen");

	/* assuming LSB environment (network byte order is canonically MSB) */
	printf("Server listening on port %i\n",
	       __builtin_bswap16((uint16_t)(server_addr.sin_port)));

	client_id = accept(server_id, (struct sockaddr *)&client_addr,
			    &client_addr_sz);
	if (client_id == -1)
		errorExit(server_id, "simpleServer: accept");
	printf("Client connected: %s\n",
	       inet_ntoa(client_addr.sin_addr));

	if (recv(client_id, (void *)recv_buf, RECV_BUFSZ, 0) == -1)
		errorExit(server_id, "simpleServer: recv");
	printf("Message received: \"%s\"\n", recv_buf);

	if (close(client_id) != 0 || close(server_id) != 0)
	{
		perror("simpleServer: close");
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}


/**
 * main - entry point for simpleServer
 *
 * Return: return value of simpleServer
 */
int main(void)
{
	return (simpleServer());
}
