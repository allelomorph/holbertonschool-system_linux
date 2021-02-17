/* printf */
#include <stdio.h>
/* signal */
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
 * handle_signal - registers a handler for SIGINT
 * note: signal() is deprecated for sigaction() and only used due to project
 * parameters
 *
 * Return: 0 on success, -1 on failure
 */
int handle_signal(void)
{
	if (signal(SIGINT, SIGINT_handler) == SIG_ERR)
		return (-1);
	return (0);
}
