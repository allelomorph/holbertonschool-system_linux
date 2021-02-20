/* sigemptyset sigaddset sigprocmask */
#include <signal.h>
/* NULL */
#include <stddef.h>

#include "signals.h"


/**
 * handle_pending - sets up a handler for all the pending signals of the
 * current process
 *
 * @handler: signal handler function to assign to pending signals
 * Return: 0 on success, or -1 on error
 */
int handle_pending(void (*handler)(int))
{
	struct sigaction sa;
	sigset_t sigset;
	int sig, retval;

	/* ensure empty set */
	if (sigemptyset(&sigset) != 0)
		return (-1);

	/* fill set with pending signals */
	if (sigpending(&sigset) != 0)
		return (-1);

	sa.sa_handler = handler;

	for (sig = 1; sig < SIGRTMIN; sig++)
	{
		retval = sigismember(&sigset, sig);
		if (retval == -1)
			return (-1);
		if (retval == 1)
			sigaction(sig, &sa, NULL);
	}

	return (0);
}
