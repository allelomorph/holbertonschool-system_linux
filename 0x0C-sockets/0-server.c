/* netinet/in.h also includes sys/types.h and sys/socket.h */
/* sys/types.h needed for socket.h portability beyond Linux / POSIX.1-2001 */
/* netinet/in.h: sockaddr_in htons htonl; sys/socket.h: socket bind listen */
#include <netinet/in.h>
/* perror printf */
#include <stdio.h>
#include <errno.h>
/* close */
#include <unistd.h>
/* exit EXIT_FAILURE EXIT_SUCCESS */
#include <stdlib.h>


#define MAX_PENDING 10


/**
 * errorExit - exits calling function with message, after closing the socket
 *   specified by fd sckt_id
 *
 * @sckt_id: fd of socket to close on exit
 * @error_msg: message to print on exit
 */
void errorExit(int sckt_id, char *error_msg)
{
	/* tries to complete transmissions first, otherwise use shutdown(2) */
	if (close(sckt_id) != 0)
	{
		perror("simpleServer: close");
		exit(EXIT_FAILURE);
	}

	if (error_msg)
		perror(error_msg);

	exit(EXIT_FAILURE);
}


/**
 * simpleServer - opens an IPv4/TCP socket, and listens to traffic on port
 *   12345 (Any address).
 *
 * Does not accept incoming connections.
 * Program will hang indefinitely, expecting to be killed by a signal.
 *
 * Return: EXIT_FAILURE or EXIT_SUCCESS
 */
int simpleServer(void)
{
	int sckt_id;
	struct sockaddr_in sckt_addr;

	/* 0 for default protocol, but TCP is 6 in /etc/protocols */
	sckt_id = socket(PF_INET, SOCK_STREAM, 0);
	if (sckt_id == -1)
	{
		perror("simpleServer: socket");
		return (EXIT_FAILURE);
	}

	/* address family, in host byte order */
	sckt_addr.sin_family = AF_INET;
	/* port number, converted to network byte order (MSB) */
	sckt_addr.sin_port = htons(12345);
	/* IPv4 address, converted to network byte order (MSB) */
	/* INADDR_ANY is wildcard for IPv4, see getaddrinfo(3) */
	sckt_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sckt_id, (struct sockaddr *)&sckt_addr,
		 sizeof(struct sockaddr_in)) == -1)
		errorExit(sckt_id, "simpleServer: bind");

	if (listen(sckt_id, MAX_PENDING) == -1)
		errorExit(sckt_id, "simpleServer: listen");

	/* assuming LSB environment */
	printf("Server listening on port %i\n",
	       __builtin_bswap16((uint16_t)(sckt_addr.sin_port)));

	/* sckt_id will not be closed in event of signal termination */
	for (;;)
	{}

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
