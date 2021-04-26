#include "hstrace.h"
/* struct stat S_* macros */
#include <sys/stat.h>
/* ptrace */
#include <sys/ptrace.h>
/* printf */
#include <stdio.h>


/**
 * printStatMode - prints S_IF* macros in bitwise flag set `st_mode` in a
 *   stat struct
 *
 * @mode: param value of `st_mode`, third member of `struct stat`
 */
void printStatMode(mode_t mode)
{
	if (S_ISREG(mode))
		printf("%s|", "S_IFREG");
	else if (S_ISDIR(mode))
		printf("%s|", "S_IFDIR");
	else if (S_ISCHR(mode))
		printf("%s|", "S_IFCHR");
	else if (S_ISBLK(mode))
		printf("%s|", "S_IFBLK");
	else if (S_ISFIFO(mode))
		printf("%s|", "S_IFIFO");
	else if (S_ISLNK(mode))
		printf("%s|", "S_IFLNK");
	else if (S_ISSOCK(mode))
		printf("%s|", "S_IFSOCK");

	printf("%04o, ", (mode & 0777));
}


/**
 * printStatParam - after the successful return of fstat(2) and the population
 *   of `struct stat *buf` with values, prints `st_mode` and `st_size` from
 *   that struct
 *
 * @child_pid: process id of traced child
 * @param: pointer to stat struct
 */
void printStatParam(pid_t child_pid, unsigned long param)
{
	mode_t mode;
	off_t size;
	size_t mode_offs, size_offs;

	/* calculate byte offsets of stat struct members */
	mode_offs = offsetof(struct stat, st_mode);
	size_offs = offsetof(struct stat, st_size);

	mode = ptrace(PTRACE_PEEKTEXT, child_pid,
		      param + mode_offs, NULL);
	printf("{st_mode=");
	printStatMode(mode);

	size = ptrace(PTRACE_PEEKTEXT, child_pid,
		      param + size_offs, NULL);
	printf("st_size=%li, ", size);

	/*
	 * strace prints prints st_size for regular files and symbolic links,
	 * and st_rdev for other types in the following format:
	 *
	 * printf("st_rdev=makedev(%u, %u), ", major(rdev), minor(rdev));
	 */

	printf("...}");
}
