/* printf */
#include <stdio.h>
/* pid_t */
#include <sys/types.h>
/* atoi */
#include <stdlib.h>

#include "signals.h"

/**
 * main - entry point, sends SIGINT to a process defined by given PID
 *
 * @argc: number of command line args
 * @argv: array of command line arg strings
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <pid>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	if (kill((pid_t)atoi(argv[1]), SIGINT) != 0)
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}
