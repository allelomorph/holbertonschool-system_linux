#include "hstrace.h"
/* SYS_* syscall number macros */
#include <syscall.h>
/* mmap(2) macros */
#include <sys/mman.h>
/* open(2) and access(2) macros */
#include <fcntl.h>
/* printf */
#include <stdio.h>


/**
 * printFlagsInt - determines which macros to use when printing a bitwise flag
 *   set from mmap(2), open(2), or access(2)
 *
 * @syscall_n: syscall table number
 * @param_i: paramter index, or order in prototype
 * @value: paramter value
 * Return: 1 if set found and printed, 0 if not found
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
 * mmapProtPrint - prints macros in bitwise flag set for `prot` param to
 *   mmap(2)
 *
 * @prot: param value of `prot`, third param to mmap(2)
 * Return: 1 on success
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
 * mmapFlagsPrint - prints macros in bitwise flag set for `flags` param to
 *   mmap(2)
 *
 * @flags: param value of `flags`, fourth param to mmap(2)
 * Return: 1 on success
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

	/* man 2 mmap: MAP_FILE is ignored */
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

	return (1);
}


/**
 * openFlagsPrint -  prints macros in bitwise flag set for `flags` param to
 *   open(2)
 *
 * @flags: param value of `flags`, second param to open(2)
 * Return: 1 on success
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

	/* first 2 bits must be O_RDONLY 0, O_WRONLY 0x1, or O_RDWR 0x2 */
	if ((flags & 0x3) == O_RDONLY)
		printf("O_RDONLY%s", (flags & ~0x3) ? "|" : "");

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

	return (1);
}


/**
 * accessModePrint - prints macros in bitwise flag set for `flags` param to
 *   access(2)
 *
 * @mode: param value of `mode`, second param to access(2)
 * Return: 1 on success
 */
int accessModePrint(int mode)
{
	int i, mode_macro_ct = 4;
	int mode_flags[] = {F_OK, X_OK, W_OK, R_OK};
	char *mode_macros[] = {"F_OK", "X_OK", "W_OK", "R_OK"};

	/* print F_OK 0 if no other flags set */
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
