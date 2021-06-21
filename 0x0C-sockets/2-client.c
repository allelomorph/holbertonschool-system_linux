#include <errno.h>
/* strtol EXIT_FAILURE EXIT_SUCCESS */
#include <stdlib.h>


/**
 * TBD -  connects to a listening server
 *
 * Program will hang indefinitely, expecting to be killed by a signal.
 * Does not accept incoming connections.
 */




/**
 * main -  entry point for _client
 *
 * @argc: count of command line arguments
 * @argv: array of command line arguments
 */
int main(int argc, char *argv[])
{
	char *host = NULL;
	unsigned short int port = 0;

	if (argc != 3)
	{
		printf("Usage: %s <host> <port>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	port = strtol(argv[2], NULL, 10);
	if (errno != 0 || port < 0)
	{
		printf("Invalid port number:%s\n", argv[2]);
		return (EXIT_FAILURE);
	}

	host = argv[1];

	return (EXIT_SUCCESS);
}
