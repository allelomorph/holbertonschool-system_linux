#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "signals.h"

/* Our function */
int _signals_block(int *signals);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	int signals_to_block[] = {
		SIGINT,
		SIGQUIT,
		SIGSEGV,
		SIGTRAP,
		0
	};
	int signals_to_unblock[] = {
		SIGINT,
		SIGSEGV,
		SIGTRAP,
		0
	};

	if (_signals_block(signals_to_block) == -1)
	{
		fprintf(stderr, "Failed to block signals\n");
		return (EXIT_FAILURE);
	}
	if (signals_unblock(signals_to_unblock) == -1)
	{
		fprintf(stderr, "Failed to block signals\n");
		return (EXIT_FAILURE);
	}
	while (98)
	{
		sleep(5);
	}
	return (EXIT_SUCCESS);
}
