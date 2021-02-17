/* printf */
#include <stdio.h>
/* strsignal */
#include <string.h>
/* atoi */
#include <stdlib.h>

#include "signals.h"

/**
 * main - entry point, prints strsignal description for a given signum
 *
 * @argc: number of command line args
 * @argv: array of command line arg strings
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <signum>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	printf("%s: %s\n", argv[1], strsignal(atoi(argv[1])));

	return (EXIT_SUCCESS);
}
