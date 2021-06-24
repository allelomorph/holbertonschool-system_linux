/* netinet/in.h also includes sys/types.h and sys/socket.h */
/* sys/types.h needed for socket.h portability beyond Linux / POSIX.1-2001 */
/* sockaddr_in htons htonl; sys/socket.h: socket bind listen accept recv */
#include <netinet/in.h>
/* addrinfo getaddrinfo freeaddrinfo*/
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
 * getIPv4Addrs - TCP/IPv4-sepcific wrapper for getaddrinfo, gets list of
 *   candidate addresses for a given hostname
 *
 * @hostname: hostname or IP address in IPv4 format
 * @port: port number
 * Return: pointer to list of candidate addresses, or NULL on failure
 */
struct addrinfo *getIPv4Addrs(const char *hostname, const char *port)
{
	int gai_retval;
	struct addrinfo hints;
	struct addrinfo *host_ai;

	if (!hostname || !port)
	{
		fprintf(stderr, "getIPv4Addrs: NULL paramter\n");
		return (NULL);
	}

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = IPPROTO_TCP;

	gai_retval = getaddrinfo(hostname, port, &hints, &host_ai);
	if (gai_retval != 0)
	{
		fprintf(stderr, "getIPv4Addrs: getaddrinfo: %s\n",
			gai_strerror(gai_retval));
		return (NULL);
	}

	return (host_ai);
}


/**
 * testIPAddrs - walks a list of addrinfo structs, testing socket creation and
 *   connection to each. Once a connection is established with the server,
 *   prints a message and exits
 *
 * @host_ai: SLL of structs containing address candidate matches for hostname
 * @hostname: hostname or IP address in IPv4 format
 * @port: port number
 * Return: socket fd for first successful connection, or -1 on failure
 */
int testIPAddrs(struct addrinfo *host_ai,
		const char *hostname, const char *port)
{
	int client_id;
	struct addrinfo *ai_temp;

	if (!host_ai || !hostname)
	{
		fprintf(stderr, "testIPAddrs: NULL parameters\n");
		return (-1);
	}

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
			perror("testIPAddrs: close");
			return (-1);
		}
	}

	if (!ai_temp)
	{
		fprintf(stderr,
			"testIPAddrs: no valid address found for '%s'\n",
			hostname);
		return (-1);
	}

	return (client_id);
}


/**
 * simpleClient - connects to a listening server
 *
 * Once a connection is established with the server, prints a message and exits
 *
 * @hostname: hostname or IP address in IPv4 format
 * @port: port number
 * Return: EXIT_FAILURE or EXIT_SUCCESS
 */
int simpleClient(const char *hostname, const char *port)
{
	int client_id;
	struct addrinfo *host_ai;

	if (!hostname || !port)
	{
		fprintf(stderr, "simpleClient: NULL paramter\n");
		return (EXIT_FAILURE);
	}

	host_ai = getIPv4Addrs(hostname, port);
	if (!host_ai)
		return (EXIT_FAILURE);

	client_id = testIPAddrs(host_ai, hostname, port);
	freeaddrinfo(host_ai);
	if (client_id == -1)
		return (EXIT_FAILURE);

	if (close(client_id) != 0)
	{
		perror("simpleClient: close");
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}


/**
 * main - entry point for simpleClient
 *
 * @argc: count of command line arguments
 * @argv: array of command line arguments
 * Return: return value of simpleClient, or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <host> <port>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	return (simpleClient(argv[1], argv[2]));
}
