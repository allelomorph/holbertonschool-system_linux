#ifndef SIGNALS_H
#define SIGNALS_H

/* siginfo_t */
#include <signal.h>

/* from man 2 signal */
typedef void (*sighandler_t)(int);

/* SIGINT handler from tasks 0 and 2 */
void SIGINT_handler(int signum);

/* 0. Handle signal */
int handle_signal(void);

/* 1. Current handler - signal */
void (*current_handler_signal(void))(int);

/* 2. Gotta catch them all */
int handle_sigaction(void);

/* 3. Current handler - sigaction */
void (*current_handler_sigaction(void))(int);

/* 4. Who said that?! */
void SIGQUIT_handler(int signum, siginfo_t *si, void *ucontext);
int trace_signal_sender(void);

/* 10. Does it exist? */
int pid_exist(pid_t pid);

/* 12. I'm saying it's unkillable. */
void all_in_one(void);

/* 13. Sigset */
int sigset_init(sigset_t *set, int *signals);

/* 14. Block signals */
int signals_block(int *signals);

/* 15. Unblock signals */
int signals_unblock(int *signals);

/* 16. Handle pending signals */
int handle_pending(void (*handler)(int));

#endif /* SIGNALS_H */
