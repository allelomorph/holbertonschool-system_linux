#include "hstrace.h"
/* SYS_* syscall number macros */
#include <syscall.h>
/* printf */
#include <stdio.h>


/**
 * printParams - tbd
 *
 * @regs: pointer to user_regs_struct containing latest registers queried
 *   from tracee
 * @child_pid: tbd
 */
void printParams(struct user_regs_struct *regs, pid_t child_pid)
{
	size_t i;
	unsigned long param;
	syscall_t syscall = syscalls_64[regs->orig_rax];

	if (!regs)
		return;

	for (i = 0; i < syscall.n_params; i++)
	{
		if (syscall.params[i] == VOID)
			continue;

		switch (i)
		{
		case 0:
			param = (unsigned long)regs->rdi;
			break;
		case 1:
			param = (unsigned long)regs->rsi;
			break;
		case 2:
			param = (unsigned long)regs->rdx;
			break;
		case 3:
			param = (unsigned long)regs->r10;
			break;
		case 4:
			param = (unsigned long)regs->r8;
			break;
		case 5:
			param = (unsigned long)regs->r9;
			break;
		default:
			return;
		}

		printParam(child_pid, regs, param, i);
		printf("%s", (i < syscall.n_params - 1) ? ", " : "");
	}
}


/**
 * printParam - tbd
 *
 * @child_pid: tbd
 * @regs: pointer to user_regs_struct containing latest registers queried
 *   from tracee
 * @param: tbd
 * @i: tbd
 */
void printParam(pid_t child_pid, struct user_regs_struct *regs,
		unsigned long param, size_t i)
{
	syscall_t syscall = syscalls_64[regs->orig_rax];

	if (!regs)
		return;

	switch (syscall.params[i])
	{
	case CHAR_P:
		_printStrParam(child_pid, param);
		break;
	case VARARGS:
		printf("...");
			break;
	case VOID:
		break;
	default:
		printf("%#lx", param);
		break;
	}
}
