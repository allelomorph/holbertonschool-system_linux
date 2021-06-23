/* USHRT_MAX */
/* #include <limits.h> */
/* netinet/in.h also includes sys/types.h and sys/socket.h */
/* sys/types.h needed for socket.h portability beyond Linux / POSIX.1-2001 */
/* sockaddr_in htons htonl; sys/socket.h: socket bind listen accept recv */
#include <netinet/in.h>
/* addrinfo */
#include <netdb.h>
/* close */
#include <unistd.h>
/* exit EXIT_FAILURE EXIT_SUCCESS */
#include <stdlib.h>
/* memset */
#include <string.h>
/* printf fprintf */
#include <stdio.h>


/**
 * simpleClient -  connects to a listening server
 *
 * Program will hang indefinitely, expecting to be killed by a signal.
 * Does not accept incoming connections.
 */
int simpleClient(const char *hostname, const char *port)
{
	int client_id, gai_retval;
	struct addrinfo hints;
	struct addrinfo *host_ai, *ai_temp;

	if (!hostname || !port)
	{
		fprintf(stderr, "simpleClient: NULL paramter\n");
		return (EXIT_FAILURE);
	}

	/* Obtain address(es) matching host/port */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = IPPROTO_TCP;

	gai_retval = getaddrinfo(hostname, port, &hints, &host_ai);
	if (gai_retval != 0)
	{
		fprintf(stderr, "simpleClient: getaddrinfo: %s\n",
			gai_strerror(gai_retval));
	        return (EXIT_FAILURE);
	}

	/* getaddrinfo() returns a list of address structures.
              Try each address until we successfully connect(2).
              If socket(2) (or connect(2)) fails, we (close the socket
              and) try the next address. */

	for (ai_temp = host_ai; ai_temp != NULL; ai_temp = ai_temp->ai_next)
	{
		client_id = socket(ai_temp->ai_family, ai_temp->ai_socktype,
			     ai_temp->ai_protocol);
		if (client_id == -1)
			continue;

		if (connect(client_id, ai_temp->ai_addr,
			    ai_temp->ai_addrlen) != -1)
		{
			printf("Connected to %s:%s\n", hostname, port);
			break;
		}

		if (close(client_id) != 0)
		{
			perror("simpleClient: close");
		        return (EXIT_FAILURE);
		}
	}

	if (ai_temp == NULL)
	{
		fprintf(stderr, "simpleClient: Could not connect to any IP "
			"address associated with %s\n", hostname);
	        return (EXIT_FAILURE);
	}
	freeaddrinfo(host_ai);

	if (close(client_id) != 0)
	{
		perror("simpleClient: close");
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}


/**
 * main -  entry point for simpleClient
 *
 * @argc: count of command line arguments
 * @argv: array of command line arguments
 */
int main(int argc, char *argv[])
{
/*
	char *host = NULL;
	unsigned short int port = 0;
	int _port;
*/
	if (argc != 3)
	{
		printf("Usage: %s <host> <port>\n", argv[0]);
		return (EXIT_FAILURE);
	}
/*
	_port = atoi(argv[2]);
	if (errno != 0 || _port < 0 || _port > USHRT_MAX)
	{
		printf("Invalid port number:%s\n", argv[2]);
		return (EXIT_FAILURE);
	}

	port = (unsigned short int)_port;
	host = argv[1];
*/
	return (simpleClient(argv[1], argv[2]));
}
