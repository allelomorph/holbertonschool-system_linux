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
 * signals_block - blocks a given set of signals from being delivered to the
 * current process
 * note: this could be done with sigblock, sigaction, or sigprocmask.
 * sigblock is for the old BSD API, and sigaction is not allowed by project
 *
 * @signals: array of signal numbers to add to set
 * Return: 0 on success, or -1 on error
 */
int signals_block(int *signals)
{
	sigset_t sigset;

	if (sigset_init(&sigset, signals) != 0)
		return (-1);

	/* SIG_BLOCK: add sigset to existing blocked signals of this thread */
	return (sigprocmask(SIG_BLOCK, &sigset, NULL));
}
