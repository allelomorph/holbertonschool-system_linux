/* sigaction */
#include <signal.h>
/* NULL */
#include <stddef.h>

#include "signals.h"

/**
 * current_handler_sigaction - retrieves the current handler of SIGINT
 *
 * Return: pointer to current handler function for SIGINT
 */
void (*current_handler_sigaction(void))(int)
{
	struct sigaction curr_sa;

	if (sigaction(SIGINT, NULL, &curr_sa) == -1)
		return (NULL);

	/*
	 * man sigaction makes it sound like the second arg is ignored if NULL,
	 * but in tests this resets the handler to SIG_DFL, so we restore it:
	 */
	if (sigaction(SIGINT, &curr_sa, NULL) == -1)
		return (NULL);

	return (curr_sa.sa_handler);
}
