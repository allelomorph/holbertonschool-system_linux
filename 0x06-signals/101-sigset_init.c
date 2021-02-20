/* sigemptyset sigaddset */
#include <signal.h>

#include "signals.h"


/**
 * sigset_init - initializes a signal set
 *
 * @set: pointer to signal set to init
 * @signals: array of signal numbers to add to set
 * Return: 0 on success, or -1 on error
 */
int sigset_init(sigset_t *set, int *signals)
{
	int i;

	if (sigemptyset(set) == -1)
		return (-1);

	/* signals is 0-terminated */
	for (i = 0; signals[i]; i++)
	{
		if (sigaddset(set, signals[i]) != 0)
			return (-1);
	}

	return (0);
}
