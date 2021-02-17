/* printf */
#include <stdio.h>
/* sigaction */
#include <signal.h>

#include "signals.h"

/**
 * SIGQUIT_handler - prints message on when calling function recieves SIGQUIT
 * note: printf is not async-safe, and only used here for the simple exercise:
 * see signal(7)
 *
 * @signum: code for incoming signal
 * @si: pointer to struct containing signal info
 * @ucontext: rarely used third arg to sa_sigaction, see getcontext(3)
 */
void SIGQUIT_handler(int signum, siginfo_t *si, void *ucontext)
{
	(void)ucontext;

	if (signum == SIGQUIT)
		printf("SIGQUIT sent by %u\n", si->si_pid);
}


/**
 * trace_signal_sender - registers a handler for SIGINT
 *
 * Return: 0 on success, -1 on failure
 */
int trace_signal_sender(void)
{
	struct sigaction sa;

	sa.sa_sigaction = SIGQUIT_handler;

	return (sigaction(SIGQUIT, &sa, NULL));
}
