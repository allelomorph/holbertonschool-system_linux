/* ptrace */
#include <sys/ptrace.h>
/* wait */
#include <sys/wait.h>
/* pid_t major() minor() */
#include <sys/types.h>
/* execve */
#include <unistd.h>
/* struct user_regs_struct */
#include <sys/user.h>
/* printf */
#include <stdio.h>
/* syscalls_64 syscall_t */
#include "syscalls.h"
/* SYS_* syscall number macros */
#include <syscall.h>
/* strlen */
#include <string.h>
/* mmap(2) macros */
#include <sys/mman.h>
/* open(2) and access(2) macros */
#include <fcntl.h>
/* struct stat S_* macros */
#include <sys/stat.h>

/* getoffsetof */
#include <stddef.h>
#include "hstrace.h"


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
