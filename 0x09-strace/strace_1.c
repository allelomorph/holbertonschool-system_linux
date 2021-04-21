/* ptrace */
#include <sys/ptrace.h>
/* wait */
#include <sys/wait.h>
/* pid_t */
#include <sys/types.h>
/* execve */
#include <unistd.h>
/* struct user_regs_struct */
#include <sys/user.h>
/* printf */
#include <stdio.h>
/* syscalls_64 */
#include "syscalls.h"


/**
 * tracerLoop - queries registers after successful execve in child, and at
 *   every "syscall-enter-stop" to print syscall name. Assumes 64-bit
 *   implementation.
 *
 * @child_pid: process ID of tracee/child
 * Return: 0 on success, 1 on failure
 */
int tracerLoop(pid_t child_pid)
{
	int status, syscall_return, first_syscall;
	struct user_regs_struct regs;

	syscall_return = 1; /* first wait is after execve return in child */
	first_syscall = 1; /* copied execve counts as first syscall of child */

	while (1)
	{
		if (wait(&status) == -1)
			return (1);
		if (WIFEXITED(status))
		{
			putchar('\n');
			break;
		}

		if (!syscall_return || first_syscall)
		{
			if (ptrace(PTRACE_GETREGS, child_pid,
				   NULL, &regs) == -1)
				return (1);

			printf("%s", syscalls_64[regs.orig_rax].name);
			fflush(stdout);
			first_syscall = 0;
		}

		if (syscall_return)
			putchar('\n');

		if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1)
			return (1);

		/* wait will return after every syscall entry and return */
		syscall_return = syscall_return ? 0 : 1;
	}

	return (0);
}


/**
 * main - entry point for strace_1
 *
 * @argc: count of command line parameters
 * @argv: array of command line parameters
 * @envp: array of environmental variables
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char *argv[], char *envp[])
{
	pid_t pid;

	if (argc < 2 || !argv)
	{
		fprintf(stderr, "usage: %s <prog> <prog args>...\n", argv[0]);
		return (1);
	}

	switch (pid = fork())
	{
	case -1:
		return (1);
	case 0: /* child/tracee */
		if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
			return (1);
		if (execve(argv[1], argv + 1, envp) == -1)
			return (1);
	default:
		break;
	}

	return (tracerLoop(pid));
}
