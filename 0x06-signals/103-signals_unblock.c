/* sigemptyset sigaddset sigprocmask */
#include <signal.h>
/* NULL */
#include <stddef.h>

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


/**
 * signals_unblock - blocks a given set of signals from being delivered to the
 * current process
 * note: this could be done with sigsetmask, sigaction, or sigprocmask.
 * sigsetmask is for the old BSD API, and sigaction is not allowed by project
 *
 * @signals: array of signal numbers to remove from blocked signals
 * Return: 0 on success, or -1 on error
 */
int signals_unblock(int *signals)
{
	sigset_t sigset;

	if (sigset_init(&sigset, signals) != 0)
		return (-1);

	/* SIG_UNBLOCK: remove sigset from blocked signals of this thread */
	return (sigprocmask(SIG_UNBLOCK, &sigset, NULL));
}
