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

/* man 2 mmap: MAP_FILE 0x0 not used
       MAP_FILE
              Compatibility flag.  Ignored.
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


void printStrParam(pid_t child_pid, size_t syscall_n,
		   unsigned long param, size_t count)
{
        size_t i;
	char c;
	unsigned char uc;

	if (syscall_n == SYS_read ||
	    syscall_n == SYS_write)
	{
		/* print bytes by looping PEEKTEXT, 32 char / byte ct truncation */
		putchar('\"');
		for (i = 0; i < count && i < 32; i++)
		{
			uc = ptrace(PTRACE_PEEKTEXT, child_pid,
				    param + i, NULL);
/*
			printf("|%u|", uc);
*/
			printRWBuffChar(uc);
		}
		putchar('\"');
		if (i == 32)
			printf("...");
	}
	else
	{
		/* print string by looping PEEKTEXT, no truncation */
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

/*
		S_ISLNK(m)  symbolic link?  (Not in POSIX.1-1996.)
		S_ISSOCK(m) socket?  (Not in POSIX.1-1996.)
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


void printStructStatParam(pid_t child_pid, unsigned long param)
{
	mode_t mode;
	dev_t rdev;    /* device ID (if special file) */
	off_t size;
	size_t mode_offs, size_offs, rdev_offs;

	/* calculate byte offsets of stat struct members */
	mode_offs = offsetof(struct stat, st_mode);
        size_offs = offsetof(struct stat, st_size);
        rdev_offs = offsetof(struct stat, st_rdev);

	mode = ptrace(PTRACE_PEEKTEXT, child_pid,
		      param + mode_offs, NULL);
	printf("{st_mode=");
	printStatMode(mode);

	/* regular files and symbolic links have st_size */
	if (S_ISREG(mode) || S_ISLNK(mode))
	{
	        size = ptrace(PTRACE_PEEKTEXT, child_pid,
			      param + size_offs, NULL);
		printf("st_size=%li, ", size);
	}
	else /* print reconstructed makedev call */
	{
	        rdev = ptrace(PTRACE_PEEKTEXT, child_pid,
			      param + rdev_offs, NULL);
		/* makedev-related macros */
		printf("st_rdev=makedev(%u, %u), ", major(rdev), minor(rdev));
	}

	printf("...}");
}

#ifdef ZZZ
           struct stat {
	   dev_t     st_dev;     /* ID of device containing file */
	ino_t     st_ino;     /* inode number */
	mode_t    st_mode;    /* protection */
	nlink_t   st_nlink;   /* number of hard links */
	uid_t     st_uid;     /* user ID of owner */
	gid_t     st_gid;     /* group ID of owner */
	dev_t     st_rdev;    /* device ID (if special file) */
	off_t     st_size;    /* total size, in bytes */
	blksize_t st_blksize; /* blocksize for filesystem I/O */
	blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
	time_t    st_atime;   /* time of last access */
	time_t    st_mtime;   /* time of last modification */
	time_t    st_ctime;   /* time of last status change */
};
#endif


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
			printStrParam(child_pid, syscall.table_n,
				      param, (size_t)regs->rdx);
			break;
		case STRUCT_STAT_P:
			/*
			 * only printing on syscall-exit-stop - retval should
			 * default to -38 on syscall-enter-stop
			 */
			if (regs->rax == 0)
				printStructStatParam(child_pid, param);
			break;
		case VARARGS:
			printf("...");
		        break;
		case VOID:
			break;
		case VOID_P:
			if (syscall.table_n == SYS_read ||
			    syscall.table_n == SYS_write)
			{
				printStrParam(child_pid, syscall.table_n,
					      param, (size_t)regs->rdx);
				break;
			}
		default:
			printf("%#lx", param);
			break;
		}
	}
}


/**
 * printParams - prints registers containing syscall parameters as hex values
 *   in ", " delimited series. Assumes 64-bit implementation.
 *
 * @regs: pointer to user_regs_struct containing latest registers queried
 *   from tracee
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

int lateParamRead(size_t syscall_n)
{
	/* stat, lstat, fstat calls set struct stat *buf after return */
	/* read calls set void *buf after return */

	switch (syscall_n)
	{
	case SYS_stat:
	case SYS_fstat:
	case SYS_lstat:
	case SYS_read:
		return (1);
	default:
		break;
	}

	return (0);
}


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
 * tracerLoop - queries registers after successful execve in child, and at
 *   every "syscall-enter-stop" to print syscall name and parameters, and at
 *   every "syscall-exit-stop" to print return value. Assumes 64-bit
 *   implementation.
 *
 * @child_pid: process ID of tracee/child
 * Return: 0 on success, 1 on failure
 */
int tracerLoop(pid_t child_pid, int argc, char *argv[], char *envp[])
{
	int status, syscall_return, first_syscall;
	struct user_regs_struct regs;

	first_syscall = 1; /* copied execve counts as first syscall of child */
	syscall_return = 1; /* first wait is after execve return in child */

	while (1)
	{
		if (wait(&status) == -1)
			return (1);
		if (WIFEXITED(status))
		{
			printf(") = ?\n");
			break;
		}
		if (ptrace(PTRACE_GETREGS, child_pid, NULL, &regs) == -1)
			return (1);
		if (first_syscall)
		{
			printf("%s(", syscalls_64[regs.orig_rax].name);
			printExecveParams(argc, argv, envp);
			first_syscall = 0;
		}
		if (syscall_return)
		{ /* retval of stat calls determines printing of stat struct */
			if (lateParamRead(syscalls_64[regs.orig_rax].table_n))
				printParams(&regs, child_pid);
			printReturn(&regs);
		}
		else
		{
			printf("%s(", syscalls_64[regs.orig_rax].name);
			if (!lateParamRead(syscalls_64[regs.orig_rax].table_n))
				printParams(&regs, child_pid);
			fflush(stdout);
		}
		if (ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL) == -1)
			return (1);
		/* wait will return after every syscall entry and return */
		syscall_return = syscall_return ? 0 : 1;
	}
	return (0);
}


/**
 * main - entry point for strace_8
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
