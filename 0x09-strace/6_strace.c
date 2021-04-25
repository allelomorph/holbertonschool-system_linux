#include "hstrace.h"
/* SYS_* syscall number macros */
#include <syscall.h>
/* ptrace */
#include <sys/ptrace.h>
/* wait */
#include <sys/wait.h>
/* execve */
#include <unistd.h>
/* printf */
#include <stdio.h>
/* strlen */
#include <string.h>


/**
 * printReturn - prints syscall return values as integer type or raw hex
 *
 * @regs: pointer to struct containing current register values in child
 */
void printReturn(struct user_regs_struct *regs)
{
	syscall_t syscall = syscalls_64[regs->orig_rax];
	char *format = NULL;

	if (!regs)
		return;

	switch (syscall.ret_t)
	{
	case INT:
	case PID_T:
		format = ") = %i\n";
		break;
	case LONG:
	case SSIZE_T:
		format = ") = %li\n";
		break;
	case UNSIGNED_INT:
	case UINT32_T:
		format = ") = %u\n";
		break;
	case UNSIGNED_LONG:
	case U64:
	case SIZE_T:
		format = ") = %lu\n";
		break;
	default:
		format = ") = %#lx\n";
		break;
	}

	printf(format, regs->rax);
}


/**
 * printExecveParams - the parameters sent into execve in the parent are not
 *   visible to the child once the memory image is copied into the new
 *   process. This function directly prints them from their values in the
 *   parent.
 *
 * @argc: count of command line parameters
 * @argv: array of command line parameters
 * @envp: array of environmental variables
 * Return: 0 on success, 1 on failure
 */
void printExecveParams(int argc, char *argv[], char *envp[])
{
	int env_ct, i;

	printf("\"%.32s\"%s, [", argv[1], strlen(argv[1]) > 32 ? "..." : "");

	for (i = 1; i < argc && i < 17; i++)
	{
		printf("\"%.32s\"%s%s", argv[i],
		       (strlen(argv[i]) > 32) ? "..." : "",
		       (i < argc - 1 && i < 16) ? ", " : "");

		if (i == 16)
			printf(", ...");
	}

	for (env_ct = 0; envp[env_ct]; env_ct++)
	{}
	printf("], [/* %i vars */]", env_ct);
}


/**
 * tracerLoop - queries registers after successful execve in child, and at
 *   every "syscall-enter-stop" to print syscall name and parameters, and at
 *   every "syscall-exit-stop" to print return value. Assumes 64-bit
 *   implementation.
 *
 * @child_pid: process ID of tracee/child
 * @argc: count of command line parameters
 * @argv: array of command line parameters
 * @envp: array of environmental variables
 * Return: 0 on success, 1 on failure
 */
int tracerLoop(pid_t child_pid, int argc, char *argv[], char *envp[])
{
	int status, syscall_return, first_syscall;
	struct user_regs_struct regs;

	if (wait(&status) == -1)
		return (1);
	first_syscall = 1; /* copied execve counts as first syscall of child */
	syscall_return = 1; /* first wait returns after execve return */

	while (!WIFEXITED(status))
	{
		if (ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) == -1)
			return (1);
		if (first_syscall)
		{
			printf("%s(", syscalls_64[regs.orig_rax].name);
			printExecveParams(argc, argv, envp);
			first_syscall = 0;
		}
		if (syscall_return)
			printReturn(&regs);
		else
		{
			printf("%s(", syscalls_64[regs.orig_rax].name);
			printParams(&regs, child_pid);
			fflush(stdout);
		}
		if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1)
			return (1);
		/* PTRACE_SYSCALL returns on every syscall entry and exit */
		syscall_return = syscall_return ? 0 : 1;
		if (wait(&status) == -1)
			return (1);
	}
	printf(") = ?\n"); /* syscall return after exit is inaccessible */
	return (0);
}


/**
 * main - entry point for strace_7
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

	return (tracerLoop(pid, argc, argv, envp));
}
