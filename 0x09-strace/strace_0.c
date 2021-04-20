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
/* fprintf */
#include <stdio.h>


/**
 * tracer_loop - queries registers after successful execve in child, and at
 *   every "syscall-enter-stop" to print syscall number to stderr
 *
 * @child_pid: process ID of tracee/child
 * Return: 0 on success, 1 on failure
 */
int tracer_loop(pid_t child_pid)
{
	int status, syscall_returned, first_syscall;
	struct user_regs_struct regs;

	syscall_returned = 1; /* first wait is after execve return in child */
	first_syscall = 1; /* execve counts as first syscall of child */

	while (1)
	{
		if (wait(&status) == -1)
			return (1);
		if (WIFEXITED(status))
			break;
		/*
		 * orig_rax holds syscall number at both stop after entry
		 * and stop after return of call
		 */
		if (!syscall_returned || first_syscall)
		{
			if (ptrace(PTRACE_GETREGS, child_pid,
				   NULL, &regs) == -1)
				return (1);

			fprintf(stderr, "%li\n", (unsigned long)regs.orig_rax);
			first_syscall = 0;
		}

		if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1)
			return (1);

		/* wait will return after every syscall entry and return */
		syscall_returned = syscall_returned ? 0 : 1;
	}

	return (0);
}


/**
 * _strace_0 - simplest version of strace clone, prints syscall numbers after
 *   successful execve in child, and at every "syscall-enter-stop"
 *
 * @argc: count of command line parameters
 * @argv: array of command line parameters
 * Return: 0 on success, 1 on failure
 */
int _strace_0(int argc, char *argv[])
{
	pid_t child_pid;

	if (argc < 2 || !argv)
		return (1);

	child_pid = fork();

	if (child_pid == -1)
		return (1);
	else if (child_pid == 0)
	{
		if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1 ||
		    execve(argv[1], argv + 1, NULL) == -1)
			return (1);
	}

	return (tracer_loop(child_pid));
}


/**
 * main - entry point for strace_0
 *
 * @argc: count of command line parameters
 * @argv: array of command line parameters
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char *argv[])
{
	if (argc < 2 || !argv)
	{
		fprintf(stderr, "%s: invalid args\n", argv[0]);
		return (1);
	}

	return (_strace_0(argc, argv));
}
