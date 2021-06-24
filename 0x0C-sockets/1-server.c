/* netinet/in.h also includes sys/types.h and sys/socket.h */
/* sys/types.h needed for socket.h portability beyond Linux / POSIX.1-2001 */
/* sockaddr_in htons htonl; sys/socket.h: socket bind listen accept */
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
 * simpleServer - opens an IPv4/TCP socket, and listens to traffic on port
 *   12345 (Any address).
 *
 * Waits for the first incoming connection, prints the IP address of the
 *   connected client, and then closes the connection.
 *
 * Return: EXIT_FAILURE or EXIT_SUCCESS
 */
int simpleServer(void)
{
	int server_id, client_id;
	struct sockaddr_in server_addr = {0}, client_addr = {0};
	socklen_t client_addr_sz = 0;

	server_id = socket(PF_INET, SOCK_STREAM, 0);
	if (server_id == -1)
	{
		perror("simpleServer: socket");
		return (EXIT_FAILURE);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(12345);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

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

	if (close(client_id) != 0 || close(server_id) != 0)
	{
		perror("simpleServer: close");
		exit(EXIT_FAILURE);
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
