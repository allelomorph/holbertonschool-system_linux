#include "hstrace.h"
/* SYS_* syscall number macros */
#include <syscall.h>
/* printf */
#include <stdio.h>


/**
 * printParams - fetches parameter values from register values in traced child
 *   process and prints them in ", "-delimited series
 *
 * @regs: pointer to user_regs_struct containing latest registers queried
 *   from tracee
 * @child_pid: process id of traced child
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
 * printParam - checks syscall number and parameter data type to determine
 *   printing format of syscall parameter
 *
 * @child_pid: process id of traced child
 * @regs: pointer to user_regs_struct containing latest registers queried
 *   from tracee
 * @param: parameter value from registers of traced child
 * @i: index value of paramter in sycall prototype
 */
void printParam(pid_t child_pid, struct user_regs_struct *regs,
		unsigned long param, size_t i)
{
	syscall_t syscall = syscalls_64[regs->orig_rax];

	if (!regs)
		return;
	if (isPtrParam(syscall.params[i]) && param == 0)
		printf("NULL");
	else if (!printInt(syscall.table_n, syscall.params[i], i, param))
	{
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
}


/**
 * isPtrParam - filters for pointer-type parameter data types
 *
 * @param_t: parameter data type macro
 * Return: 1 if parameter is pointer type, 0 if not
 */
int isPtrParam(type_t param_t)
{
	switch (param_t)
	{
		/* switch for (mostly) non-consecutive values */
	case AIO_CONTEXT_T_P:
	case CHAR_P:
	case CHAR_PP:
	case CPU_SET_T_P:
	case FD_SET_P:
	case GID_T_P:
	case INT_P:
	case LOFF_T_P:
	case LONG_P:
	case OFF_T_P:
	case SIGINFO_T_P:
	case SIGSET_T_P:
	case SIZE_T_P:
	case SOCKLEN_T_P:
	case TIMER_T_P:
	case TIME_T_P:
	case UID_T_P:
	case UNION_NFSCTL_RES_P:
	case UNSIGNED_CHAR_P:
	case UNSIGNED_INT_P:
	case UNSIGNED_LONG_P:
	case VOID_P:
	case VOID_PP:
		return (1);
	default:
		/* single test for consecutive value range */
		if (param_t >= STACK_T_P &&
		    param_t <= STRUCT___SYSCTL_ARGS_P)
			return (1);
	}

	return (0);
}


/**
 * printInt - formatted printing of all integer data type parameters
 *
 * @syscall_n: syscall table number
 * @param_t: paramter data type
 * @param_i: paramter index, or order in prototype
 * @value: paramter value
 * Return: 1 if parameter was printed, 0 if printing was skipped due to
 *   inappropriate data type or syscall number
 */
int printInt(size_t syscall_n, type_t param_t,
	     size_t param_i, unsigned long value)
{
	char *format = NULL;

	if (printFlagsInt(syscall_n, param_i, (int)value))
		return (1);

	switch (param_t)
	{
	case INT:
	case PID_T:
		format = "%i";
		break;
	case LONG:
	case SSIZE_T:
		format = "%li";
		break;
	case UNSIGNED_INT:
	case UINT32_T:
		format = "%u";
		break;
	case UNSIGNED_LONG:
	case U64:
	case SIZE_T:
		format = "%lu";
		break;
	default:
		break;
	}

	if (!format)
		return (0);
	printf(format, value);
	return (1);
}
