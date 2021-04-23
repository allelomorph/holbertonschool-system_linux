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
 * printFlagsInt - tbd
 *
 * @syscall_n: tbd
 * @param_i: tbd
 * @value: tbd
 * Return: tbd
 */
int printFlagsInt(size_t syscall_n, size_t param_i, int value)
{
	switch (syscall_n)
	{
	case SYS_mmap:
		if (param_i == 2)
			return (mmapProtPrint(value));
		else if (param_i == 3)
			return (mmapFlagsPrint(value));
		break;
	case SYS_open:
		if (param_i == 1)
			return (openFlagsPrint(value));
		break;
	case SYS_access:
		if (param_i == 1)
			return (accessModePrint(value));
	default:
		break;
	}

	return (0);
}


/**
 * mmapProtPrint - tbd
 *
 * @prot: tbd
 * Return: tbd
 */
int mmapProtPrint(int prot)
{
	int i, PROT_macro_ct = 4;
	int PROT_flags[] = {PROT_NONE, PROT_READ, PROT_WRITE, PROT_EXEC};
	char *PROT_macros[] = {"PROT_NONE", "PROT_READ",
			       "PROT_WRITE", "PROT_EXEC"};

	/* print PROT_NONE 0x0 if no other flags set */
	if (prot == PROT_NONE)
		printf("PROT_NONE");
	else
	{
		for (i = 1; i < PROT_macro_ct; i++)
		{
			if (prot & PROT_flags[i])
			{
				printf("%s", PROT_macros[i]);
				prot &= ~PROT_flags[i];
				if (prot)
					putchar('|');
			}
		}
	}

	return (1);
}


/**
 * mmapFlagsPrint - tbd
 *
 * @flags: tbd
 * Return: tbd
 */
int mmapFlagsPrint(int flags)
{
	int i, MAP_macro_ct = 15;
	int MAP_flags[] = {MAP_FILE, MAP_SHARED, MAP_PRIVATE,
			   MAP_FIXED, MAP_ANONYMOUS, MAP_32BIT,
			   MAP_GROWSDOWN, MAP_DENYWRITE,
			   MAP_EXECUTABLE, MAP_LOCKED,
			   MAP_NORESERVE, MAP_POPULATE,
			   MAP_NONBLOCK, MAP_STACK, MAP_HUGETLB};
	char *MAP_macros[] = {"MAP_FILE", "MAP_SHARED", "MAP_PRIVATE",
			      "MAP_FIXED", "MAP_ANONYMOUS", "MAP_32BIT",
			      "MAP_GROWSDOWN", "MAP_DENYWRITE",
			      "MAP_EXECUTABLE", "MAP_LOCKED",
			      "MAP_NORESERVE", "MAP_POPULATE",
			      "MAP_NONBLOCK", "MAP_STACK", "MAP_HUGETLB"};

	if (flags == MAP_FILE) /* 0x0, ignored */
		printf("MAP_FILE");
	else
	{
		for (i = 1; i < MAP_macro_ct; i++)
		{
			if (flags & MAP_flags[i])
			{
				printf("%s", MAP_macros[i]);
				flags &= ~MAP_flags[i];
				if (flags)
					putchar('|');
			}
		}
	}

	return (1);
}


/**
 * openFlagsPrint - tbd
 *
 * @flags: tbd
 * Return: tbd
 */
int openFlagsPrint(int flags)
{
	int i, O_macro_ct = 11; /* + 1 O_NDELAY alt 0x800 */
	int O_flags[] = {O_RDONLY, O_WRONLY, O_RDWR,
			 O_CREAT,
			 O_NOCTTY, O_TRUNC, O_NONBLOCK,
			 O_DIRECTORY, O_NOFOLLOW, O_CLOEXEC,
			 O_SYNC};
	char *O_macros[] = {"O_RDONLY", "O_WRONLY", "O_RDWR",
			    "O_CREAT",
			    "O_NOCTTY", "O_TRUNC", "O_NONBLOCK",
			    "O_DIRECTORY", "O_NOFOLLOW", "O_CLOEXEC",
			    "O_SYNC"};

	if (flags == O_RDONLY)
		printf("O_RDONLY");
	else
	{
		for (i = 1; i < O_macro_ct; i++)
		{
			if (flags & O_flags[i])
			{
				printf("%s", O_macros[i]);
				flags &= ~O_flags[i];
				if (flags)
					putchar('|');
			}
		}
	}

	return (1);
}


/**
 * accessModePrint - tbd
 *
 * @mode: tbd
 * Return: tbd
 */
int accessModePrint(int mode)
{
	int i, mode_macro_ct = 4;
	int mode_flags[] = {F_OK, X_OK, W_OK, R_OK};
	char *mode_macros[] = {"F_OK", "X_OK", "W_OK", "R_OK"};

	/* print F_OK 0x0 if no other flags set */
	if (mode == F_OK)
		printf("%s", "F_OK");
	else
	{
		for (i = 1; i < mode_macro_ct; i++)
		{
			if (mode & mode_flags[i])
			{
				printf("%s", mode_macros[i]);
				mode &= ~mode_flags[i];
				if (mode)
					putchar('|');
			}
		}
	}

	return (1);
}
