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
 * printStatMode - tbd
 *
 * @mode: tbd
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
 * printStatParam - tbd
 *
 * @child_pid: tbd
 * @param: tbd
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
