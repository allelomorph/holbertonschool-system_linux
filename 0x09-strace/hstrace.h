#ifndef HSTRACE_H
#define HSTRACE_H

/* syscalls_64 syscall_t */
#include "syscalls.h"
/* struct user_regs_struct */
#include <sys/user.h>
/* size_t */
#include <stddef.h>
/* mode_t */
#include <sys/types.h>



/* 8_strace.c 7_strace.c 6_strace.c 5_strace.c 4_strace.c */
void printReturn(struct user_regs_struct *regs);
int lateParamRead(size_t syscall_n);
void printExecveParams(int argc, char *argv[], char *envp[]);
int tracerLoop(pid_t child_pid, int argc, char *argv[], char *envp[]);
/* int main(int argc, char *argv[], char *envp[]) */


/* 8_printParams.c 7_printParams.c 6_printParams.c */
/* 5_printParams.c 4_printParams.c */
void printParams(struct user_regs_struct *regs, pid_t child_pid);
void printParam(pid_t child_pid, struct user_regs_struct *regs,
		unsigned long param, size_t i);
int isPtrParam(type_t param_t);

/* 8_printParams.c 7_printParams.c 6_printParams.c */
int printInt(size_t syscall_n, type_t param_t,
	     size_t param_i, unsigned long value);

/* 5_printParams.c 4_printParams.c */
int _printInt(type_t param_t, unsigned long value);


/* 8_printFlagsInt.c */
int printFlagsInt(size_t syscall_n, size_t param_i, int value);
int mmapProtPrint(int prot);
int mmapFlagsPrint(int flags);
int openFlagsPrint(int flags);
int accessModePrint(int mode);


/* 8_printStrParam.c 7_printStrParam.c */
void printStrParam(pid_t child_pid, size_t syscall_n,
		   unsigned long param, size_t count);
void printRWBuffChar(unsigned char uc);

/* 6_printStrParam.c 5_printStrParam.c 4_printStrParam.c */
void _printStrParam(pid_t child_pid, unsigned long param);


/* 8_printStatParam.c */
void printStatMode(mode_t mode);
void printStatParam(pid_t child_pid, unsigned long param);


#endif /* HSTRACE_H */
