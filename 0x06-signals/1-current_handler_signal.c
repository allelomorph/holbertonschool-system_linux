/* signal */
#include <signal.h>
/* NULL */
#include <stddef.h>

#include "signals.h"

/**
 * current_handler_signal - retrieves the current handler of the signal SIGINT
 *
 * Return: pointer to current handler function for SIGINT
 */
void (*current_handler_signal(void))(int)
{
	sighandler_t handler;

	/* arbitrarily reset SIGINT handler to get previous */
	handler = signal(SIGINT, SIG_IGN);
	if (handler == SIG_ERR)
		return (NULL);

	/* restore previous handler */
	if (signal(SIGINT, handler) == SIG_ERR)
		return (NULL);

	return (handler);
}
