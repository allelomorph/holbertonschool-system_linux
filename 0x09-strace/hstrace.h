#ifndef HSTRACE_H
#define HSTRACE_H

/* 8_strace.c */
void printReturn(struct user_regs_struct *regs);
int lateParamRead(size_t syscall_n);
void printExecveParams(int argc, char *argv[], char *envp[]);
int tracerLoop(pid_t child_pid, int argc, char *argv[], char *envp[]);
/* int main(int argc, char *argv[], char *envp[]) */

/* 8_printParams.c */
void printParams(struct user_regs_struct *regs, pid_t child_pid);
void printParam(pid_t child_pid, struct user_regs_struct *regs,
		unsigned long param, size_t i);
int isPtrParam(type_t param_t);
int printInt(size_t syscall_n, type_t param_t,
	     size_t param_i, unsigned long value);

/* 8_printFlagsInt.c */
int printFlagsInt(size_t syscall_n, size_t param_i, int value);
int mmapProtPrint(int prot);
int mmapFlagsPrint(int flags);
int openFlagsPrint(int flags);
int accessModePrint(int mode);

/* 8_printStrParam.c */
void printStrParam(pid_t child_pid, size_t syscall_n,
		   unsigned long param, size_t count);
void printRWBuffChar(unsigned char uc);

/* 8_printStatParam.c */
void printStatMode(mode_t mode);
void printStatParam(pid_t child_pid, unsigned long param);


#endif /* HSTRACE_H */
