#include "hstrace.h"
/* printf */
#include <stdio.h>
/* ptrace */
#include <sys/ptrace.h>
/* SYS_* syscall number macros */
#include <syscall.h>


/**
 * printStrParam - tbd
 *
 * @child_pid: tbd
 * @syscall_n: tbd
 * @param: tbd
 * @count: tbd
 */
void _printStrParam(pid_t child_pid, unsigned long param)
{
	size_t i;
	char c;

	putchar('\"');
	for (i = 0, c = 1; c; i++)
	{
		c = ptrace(PTRACE_PEEKTEXT, child_pid, param + i, NULL);
		if (c >= ' ' && c <= '~')
			putchar(c);
	}
	putchar('\"');
}
