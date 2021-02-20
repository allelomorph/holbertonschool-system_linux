/* sigaction psiginfo */
#include <signal.h>
/* NULL */
#include <stddef.h>

#include "signals.h"


/**
 * all_signal_handler - prints signal details when calling function recieves
 * any signal other than SIG
 *
 * @signum: code for incoming signal
 * @si: pointer to struct containing signal info
 * @ucontext: rarely used third arg to sa_sigaction, see getcontext(3)
 */
void all_signal_handler(int signum, siginfo_t *si, void *ucontext)
{
	(void)signum;
	(void)ucontext;

	psiginfo(si, "Caught");
}


/**
 * all_in_one - sets the same handler for all interceptible signals
 */
void all_in_one(void)
{
	struct sigaction sa;
	int sig;

	/* SA_SIGINFO set to use sa_sigaction and not sa_handler */
	sa.sa_flags |= SA_SIGINFO;

	sa.sa_sigaction = all_signal_handler;

	for (sig = 1; sig < SIGRTMIN; sig++)
	{
		if (sig != SIGKILL && sig != SIGSTOP)
			/* return: -1 and errno: EINVAL for sig: 32,33 */
			sigaction(sig, &sa, NULL);
	}
}
