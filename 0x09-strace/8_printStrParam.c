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
void printStrParam(pid_t child_pid, size_t syscall_n,
		   unsigned long param, size_t count)
{
	size_t i;
	char c;
	unsigned char uc;

	if (syscall_n == SYS_read ||
	    syscall_n == SYS_write)
	{
		putchar('\"');
		for (i = 0; i < count && i < 32; i++)
		{
			uc = ptrace(PTRACE_PEEKTEXT, child_pid,
				    param + i, NULL);
			printRWBuffChar(uc);
		}
		putchar('\"');
		if (i == 32)
			printf("...");
	}
	else
	{
		putchar('\"');
		for (i = 0, c = 1; c; i++)
		{
			c = ptrace(PTRACE_PEEKTEXT, child_pid, param + i, NULL);
			if (c >= ' ' && c <= '~')
				putchar(c);
		}
		putchar('\"');
	}
}


/**
 * printRWBuffChar - tbd
 *
 * @uc: tbd
 */
void printRWBuffChar(unsigned char uc)
{
	if (uc >= ' ' && uc <= '~')
		putchar(uc);
	else
	{
		putchar('\\');
		switch (uc)
		{
		case '\a':
			putchar('a');
			break;
		case '\b':
			putchar('b');
			break;
		case '\t':
			putchar('t');
			break;
		case '\n':
			putchar('n');
			break;
		case '\v':
			putchar('v');
			break;
		case '\f':
			putchar('f');
			break;
		case '\r':
			putchar('r');
			break;
		default:
			printf("%o", uc);
			break;
		}
	}
}
