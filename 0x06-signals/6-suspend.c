/* printf */
#include <stdio.h>
/* pause */
#include <unistd.h>
/* EXIT_FAILURE EXIT_SUCCESS */
#include <stdlib.h>
/* errno EINTR */
#include <errno.h>

#include "signals.h"


/**
 * SIGINT_handler - prints message on when calling function recieves SIGINT,
 * re-raises SIGINT
 * note: printf is not async-safe, and only used here for the simple exercise:
 * see man 7 signal
 *
 * @signum: code number of incoming signal
 */
void SIGINT_handler(int signum)
{
	printf("Caught %i\n", signum);

	if (sigaction(SIGINT, NULL, NULL) != 0)
	{
		fprintf(stderr,
			"SIGINT_handler: failure to reassign to SIG_DFL\n");
		return;
	}
}


/**
 * main - entry point to 6-suspend
 *
 * @argc: number of command line args
 * @argv: array of command line arg strings
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
	struct sigaction sa;

	(void)argc;

	sa.sa_handler = SIGINT_handler;
	if (sigaction(SIGINT, &sa, NULL) != 0)
	{
		fprintf(stderr,
			"%s: failure to assign to SIGINT_handler\n", argv[0]);
		return (EXIT_FAILURE);
	}

	pause();
	if (errno == EINTR)
		printf("Signal received\n");

	return (EXIT_SUCCESS);
}
