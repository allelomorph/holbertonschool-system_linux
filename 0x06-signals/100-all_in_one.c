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
 * notes: project limits function to 10 lines; SA_SIGINFO sets use of
 * sa_sigaction over sa_handler
 */
void all_in_one(void)
{
	struct sigaction sa;
	int sig;

	sa.sa_flags |= SA_SIGINFO;
	sa.sa_sigaction = all_signal_handler;
	for (sig = 1; sig < SIGRTMIN; sig++)
	{
		if (sig != SIGKILL && sig != SIGSTOP)
			sigaction(sig, &sa, NULL);
	}
}
