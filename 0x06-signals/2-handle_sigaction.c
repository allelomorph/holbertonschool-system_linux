/* printf */
#include <stdio.h>
/* sigaction */
#include <signal.h>

#include "signals.h"

/**
 * SIGINT_handler - prints message on when calling function recieves SIGINT
 * note: printf is not async-safe, and only used here for the simple exercise:
 * see man 7 signal
 *
 * @signum: code number of incoming signal
 */
void SIGINT_handler(int signum)
{
	printf("Gotcha! [%i]\n", signum);
}


/**
 * handle_sigaction - registers a handler for SIGINT
 *
 * Return: 0 on success, -1 on failure
 */
int handle_sigaction(void)
{
	struct sigaction sa;

	sa.sa_handler = SIGINT_handler;

	return (sigaction(SIGINT, &sa, NULL));
}
