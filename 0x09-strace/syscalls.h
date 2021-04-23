#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stddef.h>

/*
 * Normally, MAX_PARAMS is always 6 per the x86-64 ABI:
 * https://linux-kernel-labs.github.io/refs/heads/master/lectures/syscalls.html
 */
#define MAX_PARAMS	6

/**
 * enum type_e - enumerates the data types that appear in syscall parameters
 * and returns
 *
 * Listed includes are from Ubuntu 14.04. If given a choice in the man page,
 * POSIX.1-2001 include is listed. Some types are defined in multiple headers.
 * Sometimes actual definition will be in a wrapped header like those in bits/,
 * in which case the highest-level wrapper found is named.
 *
 *  type_t                       data type                 include to define
 *
 * @AIO_CONTEXT_T:               aio_context_t             <linux/aio_abi.h>
 * @AIO_CONTEXT_T_P:             aio_context_t *           <linux/aio_abi.h>
 * @CADDR_T:                     caddr_t                   <sys/types.h>
 * @CAP_USER_DATA_T:             cap_user_data_t           <linux/capability.h>
 * @CAP_USER_HEADER_T:           cap_user_header_t         <linux/capability.h>
 * @CHAR_P:                      char *
 * @CHAR_PP:                     char **
 * @CLOCKID_T:                   clockid_t                 <time.h>
 * @CLOCK_T:                     clock_t                   <time.h>
 * @CPU_SET_T_P:                 cpu_set_t *               <sched.h>
 * @DEV_T:                       dev_t                     <sys/types.h> or
 *                                                         <sys/stat.h>
 * @ENUM___PTRACE_REQUEST:       enum __ptrace_request     <sys/ptrace.h>
 * @FD_SET_P:                    fd_set *                  <sys/select.h>
 * @GID_T:                       gid_t                     <sys/types.h> or
 *                                                         <sys/stat.h>
 * @GID_T_P:                     gid_t *                   <sys/types.h> or
 *                                                         <sys/stat.h>
 * @IDTYPE_T:                    idtype_t                  <sys/wait.h>
 * @ID_T:                        id_t                      <sys/types.h> or
 *                                                         <sys/resource.h> or
 *                                                         <sys/wait.h>
 * @INT:                         int
 * @INT_P:                       int *
 * @KEY_SERIAL_T:                key_serial_t
 *      (keyutils.h not found, type used by add_key(2) and request_key(2))
 * @KEY_T:                       key_t                     <sys/types.h> or
 *                                                         <sys/ipc.h>
 * @LOFF_T_P:                    loff_t *                  <sys/types.h>
 * @LONG:                        long
 * @LONG_P:                      long *
 * @MODE_T:                      mode_t                    <sys/types.h> or
 *                                                         <sys/mman.h> or
 *                                                         <sys/stat.h> or
 *                                                         <sys/ipc.h> or
 *                                                         <fcntl.h>
 * @MQD_T:                       mqd_t                     <mqueue.h>
 * @NFDS_T:                      nfds_t                    <sys/poll.h>
 * @OFF64_T:                     off64_t                   <stdio.h> or
 *                                                         <sys/types.h> or
 *                                                         <unistd.h> or
 *                                                         <fcntl.h>
 * @OFF_T:                       off_t                     <stdio.h> or
 *                                                         <sys/types.h> or
 *                                                         <sys/mman.h> or
 *                                                         <sys/stat.h> or
 *                                                         <unistd.h> or
 *                                                         <fcntl.h>
 * @OFF_T_P:                     off_t *                   (see off_t)
 * @PID_T:                       pid_t                     <sched.h> or
 *                                                         <termios.h> or
 *                                                         <utmpx.h> or
 *                                                         <time.h> or
 *                                                         <sys/types.h> or
 *                                                         <sys/shm.h> or
 *                                                         <sys/msg.h> or
 *                                                         <signal.h> or
 *                                                         <unistd.h> or
 *                                                         <fcntl.h>
 * @SIGHANDLER_T:                sighandler_t              <signal.h>
 * @SIGINFO_T_P:                 siginfo_t                 <signal.h>
 * @SIGSET_T_P:                  sigset_t *                <signal.h>
 * @SIZE_T:                      size_t                    <typedef.h> or
 *                                                         <unistd.h>
 * @SIZE_T_P:                    size_t *                  <typedef.h> or
 *                                                         <unistd.h>
 * @SOCKLEN_T:                   socklen_t                 <unistd.h>
 * @SOCKLEN_T_P:                 socklen_t *               <unistd.h>
 * @SSIZE_T:                     ssize_t                   <stdio.h> or
 *                                                         <sys/types.h> or
 *                                                         <unistd.h>
 * @STACK_T_P:                   stack_t *                 <signal.h>
 * @STRUCT_EPOLL_EVENT_P:        struct epoll_event *      <linux/eventpoll.h>
 * @STRUCT_GETCPU_CACHE_P:       struct getcpu_cache *
 *     (linux/getcpu.h not found, type only used by getcpu(2))
 * @STRUCT_IOCB_P:               struct iocb *             <linux/aio_abi.h>
 * @STRUCT_IOCB_PP:              struct iocb **            <linux/aio_abi.h>
 * @STRUCT_IOVEC_P:              struct iovec *            <sys/uio.h>
 * @STRUCT_IO_EVENT_P:           struct io_event *         <linux/aio_abi.h>
 * @STRUCT_ITIMERSPEC_P:         struct itimerspec *       <time.h>
 * @STRUCT_ITIMERVAL_P:          struct itimerval          <time.h>
 * @STRUCT_KERNEL_SYM_P:         struct kernel_sym *
 *     (module.h not found, type only used by get_kernel_syms(2))
 * @STRUCT_KEXEC_SEGMENT_P:      struct kexec_segment *    <linux/kexec.h>
 * @STRUCT_LINUX_DIRENT_P:       struct linux_dirent *
 *     (No definition found for linux_dirent, but readdir(3) uses the similar
 *      struct dirent, defined in dirent.h)
 * @STRUCT_MMSGHDR_P:            struct mmsghdr *          <sys/socket.h>
 * @STRUCT_MQ_ATTR_P:            struct mq_attr *          <mqueue.h>
 * @STRUCT_MSGHDR_P:             struct msghdr *           <sys/socket.h>
 * @STRUCT_MSQID_DS_P:           struct msqid_ds *         <sys/msg.h>
 * @STRUCT_NFSCTL_ARG_P:         struct nfsctl_arg *
 *     (linux/nfsd/syscall.h not found, type only used with nfsservctl(2).
 *      man 2 nfsservctl "Since Linux 3.1, this system call no longer exists.")
 * @STRUCT_OLD_LINUX_DIRENT_P: struct old_linux_dirent *
 *     (No definition found for old_linux_dirent, superseded by getdents(2)
 *      use of struct linux_dirent, which in turn is apparently superseded by
 *      struct dirent, defined in dirent.h)
 * @STRUCT_PERF_EVENT_ATTR_P:    struct perf_event_attr *  <linux/perf_event.h>
 * @STRUCT_POLLFD_P:             struct pollfd *           <poll.h>
 * @STRUCT_RLIMIT_P:             struct rlimit *           <sys/resource.h>
 * @STRUCT_ROBUST_LIST_HEAD_P:   struct robust_list_head * <linux/futex.h>
 * @STRUCT_ROBUST_LIST_HEAD_PP:  struct robust_list_head **<linux/futex.h>
 * @STRUCT_RUSAGE_P:             struct rusage *           <linux/resource.h>
 * @STRUCT_SCHED_PARAM_P:        struct sched_param        <sched.h>
 * @STRUCT_SEMBUF_P:             struct sembuf *           <sys/sem.h>
 * @STRUCT_SHMID_DS_P:           struct shmid_ds *         <sys/shm.h>
 * @STRUCT_SIGACTION_P:          struct sigaction *        <signal.h>
 * @STRUCT_SIGEVENT_P:           struct sigevent *         <signal.h>
 * @STRUCT_SOCKADDR_P:           struct sockaddr *         <sys/socket.h>
 * @STRUCT_STATFS_P:             struct statfs *           <sys/statfs.h>
 * @STRUCT_STAT_P:               struct stat *             <sys/stat.h>
 * @STRUCT_SYSINFO_P:            struct sysinfo *          <sys/sysinfo.h>
 * @STRUCT_TIMESPEC:             struct timespec           <time.h>
 * @STRUCT_TIMESPEC_P:           struct timespec *         <time.h>
 * @STRUCT_TIMEVAL:              struct timeval            <time.h>
 * @STRUCT_TIMEVAL_P:            struct timeval *          <time.h>
 * @STRUCT_TIMEX_P:              struct timex *            <sys/timex.h>
 * @STRUCT_TIMEZONE_P:           struct timezone *         <sys/time.h>
 * @STRUCT_TMS_P:                struct tms *              <sys/times.h>
 * @STRUCT_USER_DESC_P:          struct user_desc *        <asm/ldt.h>
 * @STRUCT_USTAT_P:              struct ustat *            <sys/ustat>
 * @STRUCT_UTIMBUF_P:            struct utimbuf *          <utime.h>
 * @STRUCT_UTSNAME_P:            struct utsname *          <sys/utsname.h>
 * @STRUCT_VM86_STRUCT_P:        struct vm86_struct *      <sys/vm86.h>
 *     (sys/vm86.h is unsupported on x86-64)
 * @STRUCT___SYSCTL_ARGS_P:      struct __sysctl_args *    <sys/sysctl.h>
 * @TIMER_T:                     timer_t                   <time.h>
 * @TIMER_T_P:                   timer_t *                 <time.h>
 * @TIME_T:                      time_t                    <time.h>
 * @TIME_T_P:                    time_t *                  <time.h>
 * @U64:                         u64
 *     (Only used by lookup_dcookie(2), no definition found - seems to be a
 *      kernel space name. The closest was __u64 defined with <linux/types.h>.
 *      https://stackoverflow.com/q/30896489/ for more info.)
 * @UID_T:                       uid_t                     <pwd.h> or
 *                                                         <stropts.h> or
 *                                                         <signal.h> or
 *                                                         <unistd.h>
 * @UID_T_P:                     uit_t *                   (see uid_t)
 * @UINT32_T:                    uint32_t                  <stdint.h>
 * @UNION_NFSCTL_RES_P:          union nfsctl_res *
 *     (linux/nfsd/syscall.h not found, type only used with nfsservctl(2).
 *      man 2 nfsservctl "Since Linux 3.1, this system call no longer exists.")
 * @UNSIGNED_CHAR_P:             unsigned char *
 * @UNSIGNED_INT:                unsigned int
 * @UNSIGNED_INT_P:              unsigned int *
 *     (Not in the original version of this header. Added to replace
 *      UNSIGNED_PMSG_PRIO, UNSIGNED_PCPU, and UNSIGNED_PNODE.)
 * @UNSIGNED_LONG:               unsigned long
 * @UNSIGNED_LONG_P:             unsigned long *
 * @VARARGS:                     ...                       <stdarg.h>
 *     (va_arg, va_copy, va_end, and va_start are enabled by including
 *      stdarg.h, which gcc seems to interpret as including
 *      /usr/lib/gcc/x86_64-linux-gnu/4.8/include/stdarg.h)
 * @VOID:                        void
 * @VOID_P:                      void *
 * @VOID_PP:                     void **
 */
typedef enum type_e
{
	AIO_CONTEXT_T,
	AIO_CONTEXT_T_P,
	CADDR_T,
	CAP_USER_DATA_T,
	CAP_USER_HEADER_T,
	CHAR_P,
	CHAR_PP,
	CLOCKID_T,
	CLOCK_T,
	CPU_SET_T_P,
	DEV_T,
	ENUM___PTRACE_REQUEST,
	FD_SET_P,
	GID_T,
	GID_T_P,
	IDTYPE_T,
	ID_T,
	INT,
	INT_P,
	KEY_SERIAL_T,
	KEY_T,
	LOFF_T_P,
	LONG,
	LONG_P,
	MODE_T,
	MQD_T,
	NFDS_T,
	OFF64_T,
	OFF_T,
	OFF_T_P,
	PID_T,
	SIGHANDLER_T,
	SIGINFO_T_P,
	SIGSET_T_P,
	SIZE_T,
	SIZE_T_P,
	SOCKLEN_T,
	SOCKLEN_T_P,
	SSIZE_T,
	STACK_T_P,
	STRUCT_EPOLL_EVENT_P,
	STRUCT_GETCPU_CACHE_P,
	STRUCT_IOCB_P,
	STRUCT_IOCB_PP,
	STRUCT_IOVEC_P,
	STRUCT_IO_EVENT_P,
	STRUCT_ITIMERSPEC_P,
	STRUCT_ITIMERVAL_P,
	STRUCT_KERNEL_SYM_P,
	STRUCT_KEXEC_SEGMENT_P,
	STRUCT_LINUX_DIRENT_P,
	STRUCT_MMSGHDR_P,
	STRUCT_MQ_ATTR_P,
	STRUCT_MSGHDR_P,
	STRUCT_MSQID_DS_P,
	STRUCT_NFSCTL_ARG_P,
	STRUCT_OLD_LINUX_DIRENT_P,
	STRUCT_PERF_EVENT_ATTR_P,
	STRUCT_POLLFD_P,
	STRUCT_RLIMIT_P,
	STRUCT_ROBUST_LIST_HEAD_P,
	STRUCT_ROBUST_LIST_HEAD_PP,
	STRUCT_RUSAGE_P,
	STRUCT_SCHED_PARAM_P,
	STRUCT_SEMBUF_P,
	STRUCT_SHMID_DS_P,
	STRUCT_SIGACTION_P,
	STRUCT_SIGEVENT_P,
	STRUCT_SOCKADDR_P,
	STRUCT_STATFS_P,
	STRUCT_STAT_P,
	STRUCT_SYSINFO_P,
	STRUCT_TIMESPEC,
	STRUCT_TIMESPEC_P,
	STRUCT_TIMEVAL,
	STRUCT_TIMEVAL_P,
	STRUCT_TIMEX_P,
	STRUCT_TIMEZONE_P,
	STRUCT_TMS_P,
	STRUCT_USER_DESC_P,
	STRUCT_USTAT_P,
	STRUCT_UTIMBUF_P,
	STRUCT_UTSNAME_P,
	STRUCT_VM86_STRUCT_P,
	STRUCT___SYSCTL_ARGS_P,
	TIMER_T,
	TIMER_T_P,
	TIME_T,
	TIME_T_P,
	U64,
	UID_T,
	UID_T_P,
	UINT32_T,
	UNION_NFSCTL_RES_P,
	UNSIGNED_CHAR_P,
	UNSIGNED_INT,
	UNSIGNED_INT_P,
	UNSIGNED_LONG,
	UNSIGNED_LONG_P,
	VARARGS,
	VOID,
	VOID_P,
	VOID_PP
} type_t;

/*
 * @UNSIGNED_FLAGS:
 *     (Result of a likely man page scraping error of mbind(2) in the original
 *      version of this header. Final parameter of mbind is an unsigned int,
 *      not a unique data type. mbind entries in arrays below have been
 *      amended. In any case, numaif.h and thus mbind is not avaialble in
 *      Ubuntu 14.04.)
 * @UNSIGNED_MSG_PRIO:
 *     (Result of a likely man page scraping error of mq_timedsend(2) in the
 *      original version of this header. Fourth parameter of mq_timedsend is an
 *      unsigned int, not a unique data type. mq_timedsend entries in arrays
 *      below have been amended.)
 * @UNSIGNED_NR_EVENTS:
 *     (Result of a likely man page scraping error of io_setup(2) in the
 *      original version of this header. First parameter of io_setup is an
 *      unsigned int, not a unique data type. io_setup entries in arrays
 *      below have been amended.)
 * @UNSIGNED_NSOPS:
 *     (Result of a likely man page scraping error of semop(2)/semtimedop(2)
 *      in the original version of this header. Third parameter of
 *      semop/semtimedop is an unsigned int, not a unique data type. semop/
 *      semtimedop entries in arrays below have been amended.)
 * @UNSIGNED_PCPU:
 *     (Result of a likely man page scraping error of getcpu(2) in the
 *      original version of this header. First parameter of getcpu is an
 *      unsigned int *, not a unique data type. getcpu entries in arrays below
 *      have been amended.)
 * @UNSIGNED_PMSG_PRIO:
 *     (Result of a likely man page scraping error of mq_timedreceive(2) in the
 *      original version of this header. Fourth parameter of mq_timedreceive is
 *      an unsigned int *, not a unique data type. mq_timedreceive entries in
 *      arrays below have been amended.)
 * @UNSIGNED_PNODE:
 *     (Result of a likely man page scraping error of getcpu(2) in the
 *      original version of this header. Second parameter of getcpu is an
 *      unsigned int, not a unique data type. getcpu entries in arrays below
 *      have been amended.)
 */


/**
 * struct syscall_s - Stores information about a syscall
 *
 * @table_n: Syscall number in the syscalls table
 * @name: Name of the syscall
 * @ret_t: Return type
 * @n_params: Number of parameters
 * @params: Array of parameters types
 */
typedef struct syscall_s
{
	size_t const table_n;
	char const * const name;
	type_t const ret_t;
	size_t const n_params;
	type_t const params[MAX_PARAMS];
} syscall_t;


static syscall_t const syscalls_64[] = {
	{0,   "read", SSIZE_T, 3, {INT, VOID_P, SIZE_T, -1, -1, -1}},
	{1,   "write", SSIZE_T, 3, {INT, VOID_P, SIZE_T, -1, -1, -1}},
	{2,   "open", INT, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{3,   "close", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{4,   "stat", INT, 2, {CHAR_P, STRUCT_STAT_P, -1, -1, -1, -1}},
	{5,   "fstat", INT, 2, {INT, STRUCT_STAT_P, -1, -1, -1, -1}},
	{6,   "lstat", INT, 2, {CHAR_P, STRUCT_STAT_P, -1, -1, -1, -1}},
	{7,   "poll", INT, 3, {STRUCT_POLLFD_P, NFDS_T, INT, -1, -1, -1}},
	{8,   "lseek", OFF_T, 3, {INT, OFF_T, INT, -1, -1, -1}},
	{9,   "mmap", VOID_P, 6, {VOID_P, SIZE_T, INT, INT, INT, OFF_T}},
	{10,  "mprotect", INT, 3, {VOID_P, SIZE_T, INT, -1, -1, -1}},
	{11,  "munmap", INT, 2, {VOID_P, SIZE_T, -1, -1, -1, -1}},
	{12,  "brk", INT, 1, {VOID_P, -1, -1, -1, -1, -1}},
	{13,  "rt_sigaction", INT, 3, {INT, STRUCT_SIGACTION_P,
		STRUCT_SIGACTION_P, -1, -1, -1}},
	{14,  "rt_sigprocmask", INT, 3, {INT, SIGSET_T_P, SIGSET_T_P, -1, -1,
		-1}},
	{15,  "rt_sigreturn", INT, 1, {UNSIGNED_LONG, -1, -1, -1, -1, -1}},
	{16,  "ioctl", INT, 3, {INT, INT, VARARGS, -1, -1, -1}},
	{17,  "pread64", SSIZE_T, 4, {INT, VOID_P, SIZE_T, OFF_T, -1, -1}},
	{18,  "pwrite64", SSIZE_T, 4, {INT, VOID_P, SIZE_T, OFF_T, -1, -1}},
	{19,  "readv", SSIZE_T, 3, {INT, STRUCT_IOVEC_P, INT, -1, -1, -1}},
	{20,  "writev", SSIZE_T, 3, {INT, STRUCT_IOVEC_P, INT, -1, -1, -1}},
	{21,  "access", INT, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{22,  "pipe", INT, 1, {INT_P, -1, -1, -1, -1, -1}},
	{23,  "select", INT, 5, {INT, FD_SET_P, FD_SET_P, FD_SET_P,
		STRUCT_TIMEVAL_P, -1}},
	{24,  "sched_yield", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{25,  "mremap", VOID_P, 5, {VOID_P, SIZE_T, SIZE_T, INT, VARARGS, -1}},
	{26,  "msync", INT, 3, {VOID_P, SIZE_T, INT, -1, -1, -1}},
	{27,  "mincore", INT, 3, {VOID_P, SIZE_T, UNSIGNED_CHAR_P, -1, -1, -1}},
	{28,  "madvise", INT, 3, {VOID_P, SIZE_T, INT, -1, -1, -1}},
	{29,  "shmget", INT, 3, {KEY_T, SIZE_T, INT, -1, -1, -1}},
	{30,  "shmat", VOID_P, 3, {INT, VOID_P, INT, -1, -1, -1}},
	{31,  "shmctl", INT, 3, {INT, INT, STRUCT_SHMID_DS_P, -1, -1, -1}},
	{32,  "dup", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{33,  "dup2", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{34,  "pause", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{35,  "nanosleep", INT, 2, {STRUCT_TIMESPEC_P, STRUCT_TIMESPEC_P, -1,
		-1, -1, -1}},
	{36,  "getitimer", INT, 2, {INT, STRUCT_ITIMERVAL_P, -1, -1, -1, -1}},
	{37,  "alarm", UNSIGNED_INT, 1, {UNSIGNED_INT, -1, -1, -1, -1, -1}},
	{38,  "setitimer", INT, 3, {INT, STRUCT_ITIMERVAL_P, STRUCT_ITIMERVAL_P,
		-1, -1, -1}},
	{39,  "getpid", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{40,  "sendfile", SSIZE_T, 4, {INT, INT, OFF_T_P, SIZE_T, -1, -1}},
	{41,  "socket", INT, 3, {INT, INT, INT, -1, -1, -1}},
	{42,  "connect", INT, 3, {INT, STRUCT_SOCKADDR_P, SOCKLEN_T, -1, -1,
		-1}},
	{43,  "accept", INT, 3, {INT, STRUCT_SOCKADDR_P, SOCKLEN_T_P, -1, -1,
		-1}},
	{44,  "sendto", SSIZE_T, 6, {INT, VOID_P, SIZE_T, INT,
		STRUCT_SOCKADDR_P, SOCKLEN_T}},
	{45,  "recvfrom", SSIZE_T, 6, {INT, VOID_P, SIZE_T, INT,
		STRUCT_SOCKADDR_P, SOCKLEN_T_P}},
	{46,  "sendmsg", SSIZE_T, 3, {INT, STRUCT_MSGHDR_P, INT, -1, -1, -1}},
	{47,  "recvmsg", SSIZE_T, 3, {INT, STRUCT_MSGHDR_P, INT, -1, -1, -1}},
	{48,  "shutdown", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{49,  "bind", INT, 3, {INT, STRUCT_SOCKADDR_P, SOCKLEN_T, -1, -1, -1}},
	{50,  "listen", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{51,  "getsockname", INT, 3, {INT, STRUCT_SOCKADDR_P, SOCKLEN_T_P, -1,
		-1, -1}},
	{52,  "getpeername", INT, 3, {INT, STRUCT_SOCKADDR_P, SOCKLEN_T_P, -1,
		-1, -1}},
	{53,  "socketpair", INT, 4, {INT, INT, INT, INT_P, -1, -1}},
	{54,  "setsockopt", INT, 5, {INT, INT, INT, VOID_P, SOCKLEN_T, -1}},
	{55,  "getsockopt", INT, 5, {INT, INT, INT, VOID_P, SOCKLEN_T_P, -1}},
	{56,  "clone", INT, 5, {INT, VOID_P, INT, VOID_P, VARARGS, -1}},
	{57,  "fork", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{58,  "vfork", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{59,  "execve", INT, 3, {CHAR_P, CHAR_PP, CHAR_PP, -1, -1, -1}},
	{60,  "exit", VOID, 1, {INT, -1, -1, -1, -1, -1}},
	{61,  "wait4", PID_T, 4, {PID_T, INT_P, INT, STRUCT_RUSAGE_P, -1, -1}},
	{62,  "kill", INT, 2, {PID_T, INT, -1, -1, -1, -1}},
	{63,  "uname", INT, 1, {STRUCT_UTSNAME_P, -1, -1, -1, -1, -1}},
	{64,  "semget", INT, 3, {KEY_T, INT, INT, -1, -1, -1}},
	{65,  "semop", INT, 3, {INT, STRUCT_SEMBUF_P, UNSIGNED_INT, -1, -1,
		-1}},
	{66,  "semctl", INT, 4, {INT, INT, INT, VARARGS, -1, -1}},
	{67,  "shmdt", INT, 1, {VOID_P, -1, -1, -1, -1, -1}},
	{68,  "msgget", INT, 2, {KEY_T, INT, -1, -1, -1, -1}},
	{69,  "msgsnd", INT, 4, {INT, VOID_P, SIZE_T, INT, -1, -1}},
	{70,  "msgrcv", SSIZE_T, 5, {INT, VOID_P, SIZE_T, LONG, INT, -1}},
	{71,  "msgctl", INT, 3, {INT, INT, STRUCT_MSQID_DS_P, -1, -1, -1}},
	{72,  "fcntl", INT, 3, {INT, INT, VARARGS, -1, -1, -1}},
	{73,  "flock", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{74,  "fsync", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{75,  "fdatasync", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{76,  "truncate", INT, 2, {CHAR_P, OFF_T, -1, -1, -1, -1}},
	{77,  "ftruncate", INT, 2, {INT, OFF_T, -1, -1, -1, -1}},
	{78,  "getdents", INT, 3, {UNSIGNED_INT, STRUCT_LINUX_DIRENT_P,
		UNSIGNED_INT, -1, -1, -1}},
	{79,  "getcwd", CHAR_P, 2, {CHAR_P, SIZE_T, -1, -1, -1, -1}},
	{80,  "chdir", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{81,  "fchdir", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{82,  "rename", INT, 2, {CHAR_P, CHAR_P, -1, -1, -1, -1}},
	{83,  "mkdir", INT, 2, {CHAR_P, MODE_T, -1, -1, -1, -1}},
	{84,  "rmdir", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{85,  "creat", INT, 2, {CHAR_P, MODE_T, -1, -1, -1, -1}},
	{86,  "link", INT, 2, {CHAR_P, CHAR_P, -1, -1, -1, -1}},
	{87,  "unlink", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{88,  "symlink", INT, 2, {CHAR_P, CHAR_P, -1, -1, -1, -1}},
	{89,  "readlink", SSIZE_T, 3, {CHAR_P, CHAR_P, SIZE_T, -1, -1, -1}},
	{90,  "chmod", INT, 2, {CHAR_P, MODE_T, -1, -1, -1, -1}},
	{91,  "fchmod", INT, 2, {INT, MODE_T, -1, -1, -1, -1}},
	{92,  "chown", INT, 3, {CHAR_P, UID_T, GID_T, -1, -1, -1}},
	{93,  "fchown", INT, 3, {INT, UID_T, GID_T, -1, -1, -1}},
	{94,  "lchown", INT, 3, {CHAR_P, UID_T, GID_T, -1, -1, -1}},
	{95,  "umask", MODE_T, 1, {MODE_T, -1, -1, -1, -1, -1}},
	{96,  "gettimeofday", INT, 2, {STRUCT_TIMEVAL_P, STRUCT_TIMEZONE_P, -1,
		-1, -1, -1}},
	{97,  "getrlimit", INT, 2, {INT, STRUCT_RLIMIT_P, -1, -1, -1, -1}},
	{98,  "getrusage", INT, 2, {INT, STRUCT_RUSAGE_P, -1, -1, -1, -1}},
	{99,  "sysinfo", INT, 1, {STRUCT_SYSINFO_P, -1, -1, -1, -1, -1}},
	{100, "times", CLOCK_T, 1, {STRUCT_TMS_P, -1, -1, -1, -1, -1}},
	{101, "ptrace", LONG, 4, {ENUM___PTRACE_REQUEST, PID_T, VOID_P,
		VOID_P, -1, -1}},
	{102, "getuid", UID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{103, "syslog", INT, 3, {INT, CHAR_P, INT, -1, -1, -1}},
	{104, "getgid", GID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{105, "setuid", INT, 1, {UID_T, -1, -1, -1, -1, -1}},
	{106, "setgid", INT, 1, {GID_T, -1, -1, -1, -1, -1}},
	{107, "geteuid", UID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{108, "getegid", GID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{109, "setpgid", INT, 2, {PID_T, PID_T, -1, -1, -1, -1}},
	{110, "getppid", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{111, "getpgrp", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{112, "setsid", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{113, "setreuid", INT, 2, {UID_T, UID_T, -1, -1, -1, -1}},
	{114, "setregid", INT, 2, {GID_T, GID_T, -1, -1, -1, -1}},
	{115, "getgroups", INT, 2, {INT, GID_T_P, -1, -1, -1, -1}},
	{116, "setgroups", INT, 2, {SIZE_T, GID_T_P, -1, -1, -1, -1}},
	{117, "setresuid", INT, 3, {UID_T, UID_T, UID_T, -1, -1, -1}},
	{118, "getresuid", INT, 3, {UID_T_P, UID_T_P, UID_T_P, -1, -1, -1}},
	{119, "setresgid", INT, 3, {GID_T, GID_T, GID_T, -1, -1, -1}},
	{120, "getresgid", INT, 3, {GID_T_P, GID_T_P, GID_T_P, -1, -1, -1}},
	{121, "getpgid", PID_T, 1, {PID_T, -1, -1, -1, -1, -1}},
	{122, "setfsuid", INT, 1, {UID_T, -1, -1, -1, -1, -1}},
	{123, "setfsgid", INT, 1, {UID_T, -1, -1, -1, -1, -1}},
	{124, "getsid", PID_T, 1, {PID_T, -1, -1, -1, -1, -1}},
	{125, "capget", INT, 2, {CAP_USER_HEADER_T, CAP_USER_DATA_T, -1, -1,
		-1, -1}},
	{126, "capset", INT, 2, {CAP_USER_HEADER_T, CAP_USER_DATA_T, -1, -1,
		-1, -1}},
	{127, "rt_sigpending", INT, 1, {SIGSET_T_P, -1, -1, -1, -1, -1}},
	{128, "rt_sigtimedwait", INT, 3, {SIGSET_T_P, SIGINFO_T_P,
		STRUCT_TIMESPEC_P, -1, -1, -1}},
	{129, "rt_sigqueueinfo", INT, 3, {PID_T, INT, SIGINFO_T_P, -1, -1, -1}},
	{130, "rt_sigsuspend", INT, 1, {SIGSET_T_P, -1, -1, -1, -1, -1}},
	{131, "sigaltstack", INT, 2, {STACK_T_P, STACK_T_P, -1, -1, -1, -1}},
	{132, "utime", INT, 2, {CHAR_P, STRUCT_UTIMBUF_P, -1, -1, -1, -1}},
	{133, "mknod", INT, 3, {CHAR_P, MODE_T, DEV_T, -1, -1, -1}},
	{134, "uselib", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{135, "personality", INT, 1, {UNSIGNED_LONG, -1, -1, -1, -1, -1}},
	{136, "ustat", INT, 2, {DEV_T, STRUCT_USTAT_P, -1, -1, -1, -1}},
	{137, "statfs", INT, 2, {CHAR_P, STRUCT_STATFS_P, -1, -1, -1, -1}},
	{138, "fstatfs", INT, 2, {INT, STRUCT_STATFS_P, -1, -1, -1, -1}},
	{139, "sysfs", INT, 2, {INT, CHAR_P, -1, -1, -1, -1}},
	{140, "getpriority", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{141, "setpriority", INT, 3, {INT, INT, INT, -1, -1, -1}},
	{142, "sched_setparam", INT, 2, {PID_T, STRUCT_SCHED_PARAM_P, -1, -1,
		-1, -1}},
	{143, "sched_getparam", INT, 2, {PID_T, STRUCT_SCHED_PARAM_P, -1, -1,
		-1, -1}},
	{144, "sched_setscheduler", INT, 3, {PID_T, INT, STRUCT_SCHED_PARAM_P,
		-1, -1, -1}},
	{145, "sched_getscheduler", INT, 1, {PID_T, -1, -1, -1, -1, -1}},
	{146, "sched_get_priority_max", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{147, "sched_get_priority_min", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{148, "sched_rr_get_interval", INT, 2, {PID_T, STRUCT_TIMESPEC_P, -1,
		-1, -1, -1}},
	{149, "mlock", INT, 2, {VOID_P, SIZE_T, -1, -1, -1, -1}},
	{150, "munlock", INT, 2, {VOID_P, SIZE_T, -1, -1, -1, -1}},
	{151, "mlockall", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{152, "munlockall", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{153, "vhangup", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{154, "modify_ldt", INT, 3, {INT, VOID_P, UNSIGNED_LONG, -1, -1, -1}},
	{155, "pivot_root", INT, 2, {CHAR_P, CHAR_P, -1, -1, -1, -1}},
	{156, "_sysctl", INT, 1, {STRUCT___SYSCTL_ARGS_P, -1, -1, -1, -1, -1}},
	{157, "prctl", INT, 5, {INT, UNSIGNED_LONG, UNSIGNED_LONG,
		UNSIGNED_LONG, UNSIGNED_LONG, -1}},
	{158, "arch_prctl", INT, 2, {INT, UNSIGNED_LONG, -1, -1, -1, -1}},
	{159, "adjtimex", INT, 1, {STRUCT_TIMEX_P, -1, -1, -1, -1, -1}},
	{160, "setrlimit", INT, 2, {INT, STRUCT_RLIMIT_P, -1, -1, -1, -1}},
	{161, "chroot", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{162, "sync", VOID, 1, {VOID, -1, -1, -1, -1, -1}},
	{163, "acct", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{164, "settimeofday", INT, 2, {STRUCT_TIMEVAL_P, STRUCT_TIMEZONE_P,
		-1, -1, -1, -1}},
	{165, "mount", INT, 5, {CHAR_P, CHAR_P, CHAR_P, UNSIGNED_LONG, VOID_P,
		-1}},
	{166, "umount2", INT, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{167, "swapon", INT, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{168, "swapoff", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{169, "reboot", INT, 4, {INT, INT, INT, VOID_P, -1, -1}},
	{170, "sethostname", INT, 2, {CHAR_P, SIZE_T, -1, -1, -1, -1}},
	{171, "setdomainname", INT, 2, {CHAR_P, SIZE_T, -1, -1, -1, -1}},
	{172, "iopl", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{173, "ioperm", INT, 3, {UNSIGNED_LONG, UNSIGNED_LONG, INT, -1, -1,
		-1}},
	{174, "create_module", CADDR_T, 2, {CHAR_P, SIZE_T, -1, -1, -1, -1}},
	{175, "init_module", INT, 3, {VOID_P, UNSIGNED_LONG, CHAR_P, -1, -1,
		-1}},
	{176, "delete_module", INT, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{177, "get_kernel_syms", INT, 1, {STRUCT_KERNEL_SYM_P, -1, -1, -1, -1,
		-1}},
	{178, "query_module", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{179, "quotactl", INT, 4, {INT, CHAR_P, INT, CADDR_T, -1, -1}},
	{180, "nfsservctl", LONG, 3, {INT, STRUCT_NFSCTL_ARG_P,
		UNION_NFSCTL_RES_P, -1, -1, -1}},
	{181, "getpmsg", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{182, "putpmsg", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{183, "afs_syscall", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{184, "tuxcall", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{185, "security", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{186, "gettid", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{187, "readahead", SSIZE_T, 3, {INT, OFF64_T, SIZE_T, -1, -1, -1}},
	{188, "setxattr", INT, 5, {CHAR_P, CHAR_P, VOID_P, SIZE_T, INT, -1}},
	{189, "lsetxattr", INT, 5, {CHAR_P, CHAR_P, VOID_P, SIZE_T, INT, -1}},
	{190, "fsetxattr", INT, 5, {INT, CHAR_P, VOID_P, SIZE_T, INT, -1}},
	{191, "getxattr", SSIZE_T, 4, {CHAR_P, CHAR_P, VOID_P, SIZE_T, -1, -1}},
	{192, "lgetxattr", SSIZE_T, 4, {CHAR_P, CHAR_P, VOID_P, SIZE_T, -1,
		-1}},
	{193, "fgetxattr", SSIZE_T, 4, {INT, CHAR_P, VOID_P, SIZE_T, -1, -1}},
	{194, "listxattr", SSIZE_T, 3, {CHAR_P, CHAR_P, SIZE_T, -1, -1, -1}},
	{195, "llistxattr", SSIZE_T, 3, {CHAR_P, CHAR_P, SIZE_T, -1, -1, -1}},
	{196, "flistxattr", SSIZE_T, 3, {INT, CHAR_P, SIZE_T, -1, -1, -1}},
	{197, "removexattr", INT, 2, {CHAR_P, CHAR_P, -1, -1, -1, -1}},
	{198, "lremovexattr", INT, 2, {CHAR_P, CHAR_P, -1, -1, -1, -1}},
	{199, "fremovexattr", INT, 2, {INT, CHAR_P, -1, -1, -1, -1}},
	{200, "tkill", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{201, "time", TIME_T, 1, {TIME_T_P, -1, -1, -1, -1, -1}},
	{202, "futex", INT, 6, {INT_P, INT, INT, STRUCT_TIMESPEC_P, INT_P,
		INT}},
	{203, "sched_setaffinity", INT, 3, {PID_T, SIZE_T, CPU_SET_T_P, -1,
		-1, -1}},
	{204, "sched_getaffinity", INT, 3, {PID_T, SIZE_T, CPU_SET_T_P, -1,
		-1, -1}},
	{205, "set_thread_area", INT, 1, {STRUCT_USER_DESC_P, -1, -1, -1, -1,
		-1}},
	{206, "io_setup", INT, 2, {UNSIGNED_INT, AIO_CONTEXT_T_P, -1,
		-1, -1, -1}},
	{207, "io_destroy", INT, 1, {AIO_CONTEXT_T, -1, -1, -1, -1, -1}},
	{208, "io_getevents", INT, 5, {AIO_CONTEXT_T, LONG, LONG,
		STRUCT_IO_EVENT_P, STRUCT_TIMESPEC_P, -1}},
	{209, "io_submit", INT, 3, {AIO_CONTEXT_T, LONG, STRUCT_IOCB_PP, -1,
		-1, -1}},
	{210, "io_cancel", INT, 3, {AIO_CONTEXT_T, STRUCT_IOCB_P,
		STRUCT_IO_EVENT_P, -1, -1, -1}},
	{211, "get_thread_area", INT, 1, {STRUCT_USER_DESC_P, -1, -1, -1, -1,
		-1}},
	{212, "lookup_dcookie", INT, 3, {U64, CHAR_P, SIZE_T, -1, -1, -1}},
	{213, "epoll_create", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{214, "epoll_ctl_old", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{215, "epoll_wait_old", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{216, "remap_file_pages", INT, 5, {VOID_P, SIZE_T, INT, SSIZE_T, INT,
		-1}},
	{217, "getdents64", INT, 3, {UNSIGNED_INT, STRUCT_LINUX_DIRENT_P,
		UNSIGNED_INT, -1, -1, -1}},
	{218, "set_tid_address", LONG, 1, {INT_P, -1, -1, -1, -1, -1}},
	{219, "restart_syscall", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{220, "semtimedop", INT, 4, {INT, STRUCT_SEMBUF_P, UNSIGNED_INT,
		STRUCT_TIMESPEC_P, -1, -1}},
	{221, "fadvise64", INT, 4, {INT, OFF_T, OFF_T, INT, -1, -1}},
	{222, "timer_create", INT, 3, {CLOCKID_T, STRUCT_SIGEVENT_P,
		TIMER_T_P, -1, -1, -1}},
	{223, "timer_settime", INT, 4, {TIMER_T, INT, STRUCT_ITIMERSPEC_P,
		STRUCT_ITIMERSPEC_P, -1, -1}},
	{224, "timer_gettime", INT, 2, {TIMER_T, STRUCT_ITIMERSPEC_P, -1, -1,
		-1, -1}},
	{225, "timer_getoverrun", INT, 1, {TIMER_T, -1, -1, -1, -1, -1}},
	{226, "timer_delete", INT, 1, {TIMER_T, -1, -1, -1, -1, -1}},
	{227, "clock_settime", INT, 2, {CLOCKID_T, STRUCT_TIMESPEC_P, -1, -1,
		-1, -1}},
	{228, "clock_gettime", INT, 2, {CLOCKID_T, STRUCT_TIMESPEC_P, -1, -1,
		-1, -1}},
	{229, "clock_getres", INT, 2, {CLOCKID_T, STRUCT_TIMESPEC_P, -1, -1,
		-1, -1}},
	{230, "clock_nanosleep", INT, 4, {CLOCKID_T, INT, STRUCT_TIMESPEC_P,
		STRUCT_TIMESPEC_P, -1, -1}},
	{231, "exit_group", VOID, 1, {INT, -1, -1, -1, -1, -1}},
	{232, "epoll_wait", INT, 4, {INT, STRUCT_EPOLL_EVENT_P, INT, INT, -1,
		-1}},
	{233, "epoll_ctl", INT, 4, {INT, INT, INT, STRUCT_EPOLL_EVENT_P, -1,
		-1}},
	{234, "tgkill", INT, 3, {INT, INT, INT, -1, -1, -1}},
	{235, "utimes", INT, 2, {CHAR_P, STRUCT_TIMEVAL_P, -1, -1, -1, -1}},
	{236, "vserver", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{237, "mbind", INT, 6, {VOID_P, UNSIGNED_LONG, INT, UNSIGNED_LONG_P,
		UNSIGNED_LONG, UNSIGNED_INT}},
	{238, "set_mempolicy", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{239, "get_mempolicy", INT, 5, {INT_P, UNSIGNED_LONG_P, UNSIGNED_LONG,
		UNSIGNED_LONG, UNSIGNED_LONG, -1}},
	{240, "mq_open", MQD_T, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{241, "mq_unlink", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{242, "mq_timedsend", INT, 5, {MQD_T, CHAR_P, SIZE_T,
		UNSIGNED_INT, STRUCT_TIMESPEC_P, -1}},
	{243, "mq_timedreceive", SSIZE_T, 5, {MQD_T, CHAR_P, SIZE_T,
		UNSIGNED_INT_P, STRUCT_TIMESPEC_P, -1}},
	{244, "mq_notify", INT, 2, {MQD_T, STRUCT_SIGEVENT_P, -1, -1, -1, -1}},
	{245, "mq_getsetattr", INT, 3, {MQD_T, STRUCT_MQ_ATTR_P,
		STRUCT_MQ_ATTR_P, -1, -1, -1}},
	{246, "kexec_load", LONG, 4, {UNSIGNED_LONG, UNSIGNED_LONG,
		STRUCT_KEXEC_SEGMENT_P, UNSIGNED_LONG, -1, -1}},
	{247, "waitid", INT, 4, {IDTYPE_T, ID_T, SIGINFO_T_P, INT, -1, -1}},
	{248, "add_key", KEY_SERIAL_T, 5, {CHAR_P, CHAR_P, VOID_P, SIZE_T,
		KEY_SERIAL_T, -1}},
	{249, "request_key", KEY_SERIAL_T, 4, {CHAR_P, CHAR_P, CHAR_P,
		KEY_SERIAL_T, -1, -1}},
	{250, "keyctl", LONG, 2, {INT, VARARGS, -1, -1, -1, -1}},
	{251, "ioprio_set", INT, 3, {INT, INT, INT, -1, -1, -1}},
	{252, "ioprio_get", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{253, "inotify_init", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{254, "inotify_add_watch", INT, 3, {INT, CHAR_P, UINT32_T, -1, -1, -1}},
	{255, "inotify_rm_watch", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{256, "migrate_pages", LONG, 4, {INT, UNSIGNED_LONG, UNSIGNED_LONG_P,
		UNSIGNED_LONG_P, -1, -1}},
	{257, "openat", INT, 3, {INT, CHAR_P, INT, -1, -1, -1}},
	{258, "mkdirat", INT, 3, {INT, CHAR_P, MODE_T, -1, -1, -1}},
	{259, "mknodat", INT, 4, {INT, CHAR_P, MODE_T, DEV_T, -1, -1}},
	{260, "fchownat", INT, 5, {INT, CHAR_P, UID_T, GID_T, INT, -1}},
	{261, "futimesat", INT, 3, {INT, CHAR_P, STRUCT_TIMEVAL_P, -1, -1, -1}},
	{262, "newfstatat", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{263, "unlinkat", INT, 3, {INT, CHAR_P, INT, -1, -1, -1}},
	{264, "renameat", INT, 4, {INT, CHAR_P, INT, CHAR_P, -1, -1}},
	{265, "linkat", INT, 5, {INT, CHAR_P, INT, CHAR_P, INT, -1}},
	{266, "symlinkat", INT, 3, {CHAR_P, INT, CHAR_P, -1, -1, -1}},
	{267, "readlinkat", INT, 4, {INT, CHAR_P, CHAR_P, SIZE_T, -1, -1}},
	{268, "fchmodat", INT, 4, {INT, CHAR_P, MODE_T, INT, -1, -1}},
	{269, "faccessat", INT, 4, {INT, CHAR_P, INT, INT, -1, -1}},
	{270, "pselect6", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{271, "ppoll", INT, 4, {STRUCT_POLLFD_P, NFDS_T, STRUCT_TIMESPEC_P,
		SIGSET_T_P, -1, -1}},
	{272, "unshare", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{273, "set_robust_list", LONG, 2, {STRUCT_ROBUST_LIST_HEAD_P,
		SIZE_T, -1, -1, -1, -1}},
	{274, "get_robust_list", LONG, 3, {INT, STRUCT_ROBUST_LIST_HEAD_PP,
		SIZE_T_P, -1, -1, -1}},
	{275, "splice", SSIZE_T, 6, {INT, LOFF_T_P, INT, LOFF_T_P, SIZE_T,
		UNSIGNED_INT}},
	{276, "tee", SSIZE_T, 4, {INT, INT, SIZE_T, UNSIGNED_INT, -1, -1}},
	{277, "sync_file_range", INT, 4, {INT, OFF64_T, OFF64_T, UNSIGNED_INT,
		-1, -1}},
	{278, "vmsplice", SSIZE_T, 4, {INT, STRUCT_IOVEC_P, UNSIGNED_LONG,
		UNSIGNED_INT, -1, -1}},
	{279, "move_pages", LONG, 6, {INT, UNSIGNED_LONG, VOID_PP, INT_P,
		INT_P, INT}},
	{280, "utimensat", INT, 4, {INT, CHAR_P, STRUCT_TIMESPEC_P, INT,
		-1, -1}},
	{281, "epoll_pwait", INT, 5, {INT, STRUCT_EPOLL_EVENT_P, INT, INT,
		SIGSET_T_P, -1}},
	{282, "signalfd", INT, 3, {INT, SIGSET_T_P, INT, -1, -1, -1}},
	{283, "timerfd_create", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{284, "eventfd", INT, 2, {UNSIGNED_INT, INT, -1, -1, -1, -1}},
	{285, "fallocate", INT, 4, {INT, INT, OFF_T, OFF_T, -1, -1}},
	{286, "timerfd_settime", INT, 4, {INT, INT, STRUCT_ITIMERSPEC_P,
		STRUCT_ITIMERSPEC_P, -1, -1}},
	{287, "timerfd_gettime", INT, 2, {INT, STRUCT_ITIMERSPEC_P, -1, -1,
		-1, -1}},
	{288, "accept4", INT, 4, {INT, STRUCT_SOCKADDR_P, SOCKLEN_T_P, INT,
		-1, -1}},
	{289, "signalfd4", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{290, "eventfd2", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{291, "epoll_create1", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{292, "dup3", INT, 3, {INT, INT, INT, -1, -1, -1}},
	{293, "pipe2", INT, 2, {INT_P, INT, -1, -1, -1, -1}},
	{294, "inotify_init1", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{295, "preadv", SSIZE_T, 4, {INT, STRUCT_IOVEC_P, INT, OFF_T, -1, -1}},
	{296, "pwritev", SSIZE_T, 4, {INT, STRUCT_IOVEC_P, INT, OFF_T, -1, -1}},
	{297, "rt_tgsigqueueinfo", INT, 4, {PID_T, PID_T, INT, SIGINFO_T_P,
		-1, -1}},
	{298, "perf_event_open", INT, 5, {STRUCT_PERF_EVENT_ATTR_P, PID_T,
		INT, INT, UNSIGNED_LONG, -1}},
	{299, "recvmmsg", INT, 5, {INT, STRUCT_MMSGHDR_P, UNSIGNED_INT,
		UNSIGNED_INT, STRUCT_TIMESPEC_P, -1}},
	{300, "fanotify_init", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{301, "fanotify_mark", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{302, "prlimit64", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{303, "name_to_handle_at", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{304, "open_by_handle_at", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{305, "clock_adjtime", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{306, "syncfs", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{307, "sendmmsg", INT, 4, {INT, STRUCT_MMSGHDR_P, UNSIGNED_INT,
		UNSIGNED_INT, -1, -1}},
	{308, "setns", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{309, "getcpu", INT, 3, {UNSIGNED_INT_P, UNSIGNED_INT_P,
		STRUCT_GETCPU_CACHE_P, -1, -1, -1}},
	{310, "process_vm_readv", SSIZE_T, 6, {PID_T, STRUCT_IOVEC_P,
		UNSIGNED_LONG, STRUCT_IOVEC_P, UNSIGNED_LONG, UNSIGNED_LONG}},
	{311, "process_vm_writev", SSIZE_T, 6, {PID_T, STRUCT_IOVEC_P,
		UNSIGNED_LONG, STRUCT_IOVEC_P, UNSIGNED_LONG, UNSIGNED_LONG}},
	{312, "kcmp", INT, 5, {PID_T, PID_T, INT, UNSIGNED_LONG,
		UNSIGNED_LONG, -1}},
	{313, "finit_module", INT, 3, {INT, CHAR_P, INT, -1, -1, -1}},
	{314, "sched_setattr", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{315, "sched_getattr", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{316, "renameat2", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{317, "seccomp", -1, 0, {-1, -1, -1, -1, -1, -1}}
};


static syscall_t const syscalls_32[] = {
	{0,   "restart_syscall", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{1,   "exit", VOID, 1, {INT, -1, -1, -1, -1, -1}},
	{2,   "fork", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{3,   "read", SSIZE_T, 3, {INT, VOID_P, SIZE_T, -1, -1, -1}},
	{4,   "write", SSIZE_T, 3, {INT, VOID_P, SIZE_T, -1, -1, -1}},
	{5,   "open", INT, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{6,   "close", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{7,   "waitpid", PID_T, 3, {PID_T, INT_P, INT, -1, -1, -1}},
	{8,   "creat", INT, 2, {CHAR_P, MODE_T, -1, -1, -1, -1}},
	{9,   "link", INT, 2, {CHAR_P, CHAR_P, -1, -1, -1, -1}},
	{10,  "unlink", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{11,  "execve", INT, 3, {CHAR_P, CHAR_PP, CHAR_PP, -1, -1, -1}},
	{12,  "chdir", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{13,  "time", TIME_T, 1, {TIME_T_P, -1, -1, -1, -1, -1}},
	{14,  "mknod", INT, 3, {CHAR_P, MODE_T, DEV_T, -1, -1, -1}},
	{15,  "chmod", INT, 2, {CHAR_P, MODE_T, -1, -1, -1, -1}},
	{16,  "lchown", INT, 3, {CHAR_P, UID_T, GID_T, -1, -1, -1}},
	{17,  "break", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{18,  "oldstat", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{19,  "lseek", OFF_T, 3, {INT, OFF_T, INT, -1, -1, -1}},
	{20,  "getpid", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{21,  "mount", INT, 5, {CHAR_P, CHAR_P, CHAR_P, UNSIGNED_LONG, VOID_P,
		-1}},
	{22,  "umount", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{23,  "setuid", INT, 1, {UID_T, -1, -1, -1, -1, -1}},
	{24,  "getuid", UID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{25,  "stime", INT, 1, {TIME_T_P, -1, -1, -1, -1, -1}},
	{26,  "ptrace", LONG, 4, {ENUM___PTRACE_REQUEST, PID_T, VOID_P, VOID_P,
		-1, -1}},
	{27,  "alarm", UNSIGNED_INT, 1, {UNSIGNED_INT, -1, -1, -1, -1, -1}},
	{28,  "oldfstat", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{29,  "pause", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{30,  "utime", INT, 2, {CHAR_P, STRUCT_UTIMBUF_P, -1, -1, -1, -1}},
	{31,  "stty", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{32,  "gtty", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{33,  "access", INT, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{34,  "nice", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{35,  "ftime", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{36,  "sync", VOID, 1, {VOID, -1, -1, -1, -1, -1}},
	{37,  "kill", INT, 2, {PID_T, INT, -1, -1, -1, -1}},
	{38,  "rename", INT, 2, {CHAR_P, CHAR_P, -1, -1, -1, -1}},
	{39,  "mkdir", INT, 2, {CHAR_P, MODE_T, -1, -1, -1, -1}},
	{40,  "rmdir", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{41,  "dup", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{42,  "pipe", INT, 1, {INT_P, -1, -1, -1, -1, -1}},
	{43,  "times", CLOCK_T, 1, {STRUCT_TMS_P, -1, -1, -1, -1, -1}},
	{44,  "prof", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{45,  "brk", INT, 1, {VOID_P, -1, -1, -1, -1, -1}},
	{46,  "setgid", INT, 1, {GID_T, -1, -1, -1, -1, -1}},
	{47,  "getgid", GID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{48,  "signal", SIGHANDLER_T, 2, {INT, SIGHANDLER_T, -1, -1, -1, -1}},
	{49,  "geteuid", UID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{50,  "getegid", GID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{51,  "acct", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{52,  "umount2", INT, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{53,  "lock", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{54,  "ioctl", INT, 3, {INT, INT, VARARGS, -1, -1, -1}},
	{55,  "fcntl", INT, 3, {INT, INT, VARARGS, -1, -1, -1}},
	{56,  "mpx", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{57,  "setpgid", INT, 2, {PID_T, PID_T, -1, -1, -1, -1}},
	{58,  "ulimit", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{59,  "oldolduname", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{60,  "umask", MODE_T, 1, {MODE_T, -1, -1, -1, -1, -1}},
	{61,  "chroot", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{62,  "ustat", INT, 2, {DEV_T, STRUCT_USTAT_P, -1, -1, -1, -1}},
	{63,  "dup2", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{64,  "getppid", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{65,  "getpgrp", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{66,  "setsid", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{67,  "sigaction", INT, 3, {INT, STRUCT_SIGACTION_P,
		STRUCT_SIGACTION_P, -1, -1, -1}},
	{68,  "sgetmask", LONG, 1, {VOID, -1, -1, -1, -1, -1}},
	{69,  "ssetmask", LONG, 1, {LONG, -1, -1, -1, -1, -1}},
	{70,  "setreuid", INT, 2, {UID_T, UID_T, -1, -1, -1, -1}},
	{71,  "setregid", INT, 2, {GID_T, GID_T, -1, -1, -1, -1}},
	{72,  "sigsuspend", INT, 1, {SIGSET_T_P, -1, -1, -1, -1, -1}},
	{73,  "sigpending", INT, 1, {SIGSET_T_P, -1, -1, -1, -1, -1}},
	{74,  "sethostname", INT, 2, {CHAR_P, SIZE_T, -1, -1, -1, -1}},
	{75,  "setrlimit", INT, 2, {INT, STRUCT_RLIMIT_P, -1, -1, -1, -1}},
	{76,  "getrlimit", INT, 2, {INT, STRUCT_RLIMIT_P, -1, -1, -1, -1}},
	{77,  "getrusage", INT, 2, {INT, STRUCT_RUSAGE_P, -1, -1, -1, -1}},
	{78,  "gettimeofday", INT, 2, {STRUCT_TIMEVAL_P, STRUCT_TIMEZONE_P, -1,
		-1, -1, -1}},
	{79,  "settimeofday", INT, 2, {STRUCT_TIMEVAL_P, STRUCT_TIMEZONE_P, -1,
		-1, -1, -1}},
	{80,  "getgroups", INT, 2, {INT, GID_T_P, -1, -1, -1, -1}},
	{81,  "setgroups", INT, 2, {SIZE_T, GID_T_P, -1, -1, -1, -1}},
	{82,  "select", INT, 5, {INT, FD_SET_P, FD_SET_P, FD_SET_P,
		STRUCT_TIMEVAL_P, -1}},
	{83,  "symlink", INT, 2, {CHAR_P, CHAR_P, -1, -1, -1, -1}},
	{84,  "oldlstat", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{85,  "readlink", SSIZE_T, 3, {CHAR_P, CHAR_P, SIZE_T, -1, -1, -1}},
	{86,  "uselib", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{87,  "swapon", INT, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{88,  "reboot", INT, 4, {INT, INT, INT, VOID_P, -1, -1}},
	{89,  "readdir", INT, 3, {UNSIGNED_INT, STRUCT_OLD_LINUX_DIRENT_P,
		UNSIGNED_INT, -1, -1, -1}},
	{90,  "mmap", VOID_P, 6, {VOID_P, SIZE_T, INT, INT, INT, OFF_T}},
	{91,  "munmap", INT, 2, {VOID_P, SIZE_T, -1, -1, -1, -1}},
	{92,  "truncate", INT, 2, {CHAR_P, OFF_T, -1, -1, -1, -1}},
	{93,  "ftruncate", INT, 2, {INT, OFF_T, -1, -1, -1, -1}},
	{94,  "fchmod", INT, 2, {INT, MODE_T, -1, -1, -1, -1}},
	{95,  "fchown", INT, 3, {INT, UID_T, GID_T, -1, -1, -1}},
	{96,  "getpriority", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{97,  "setpriority", INT, 3, {INT, INT, INT, -1, -1, -1}},
	{98,  "profil", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{99,  "statfs", INT, 2, {CHAR_P, STRUCT_STATFS_P, -1, -1, -1, -1}},
	{100, "fstatfs", INT, 2, {INT, STRUCT_STATFS_P, -1, -1, -1, -1}},
	{101, "ioperm", INT, 3, {UNSIGNED_LONG, UNSIGNED_LONG, INT, -1, -1,
		-1}},
	{102, "socketcall", INT, 2, {INT, UNSIGNED_LONG_P, -1, -1, -1, -1}},
	{103, "syslog", INT, 3, {INT, CHAR_P, INT, -1, -1, -1}},
	{104, "setitimer", INT, 3, {INT, STRUCT_ITIMERVAL_P,
		STRUCT_ITIMERVAL_P, -1, -1, -1}},
	{105, "getitimer", INT, 2, {INT, STRUCT_ITIMERVAL_P, -1, -1, -1, -1}},
	{106, "stat", INT, 2, {CHAR_P, STRUCT_STAT_P, -1, -1, -1, -1}},
	{107, "lstat", INT, 2, {CHAR_P, STRUCT_STAT_P, -1, -1, -1, -1}},
	{108, "fstat", INT, 2, {INT, STRUCT_STAT_P, -1, -1, -1, -1}},
	{109, "olduname", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{110, "iopl", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{111, "vhangup", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{112, "idle", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{113, "vm86old", INT, 1, {STRUCT_VM86_STRUCT_P, -1, -1, -1, -1, -1}},
	{114, "wait4", PID_T, 4, {PID_T, INT_P, INT, STRUCT_RUSAGE_P, -1, -1}},
	{115, "swapoff", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{116, "sysinfo", INT, 1, {STRUCT_SYSINFO_P, -1, -1, -1, -1, -1}},
	{117, "ipc", INT, 6, {UNSIGNED_INT, INT, INT, INT, VOID_P, LONG}},
	{118, "fsync", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{119, "sigreturn", INT, 1, {UNSIGNED_LONG, -1, -1, -1, -1, -1}},
	{120, "clone", INT, 5, {INT, VOID_P, INT, VOID_P, VARARGS, -1}},
	{121, "setdomainname", INT, 2, {CHAR_P, SIZE_T, -1, -1, -1, -1}},
	{122, "uname", INT, 1, {STRUCT_UTSNAME_P, -1, -1, -1, -1, -1}},
	{123, "modify_ldt", INT, 3, {INT, VOID_P, UNSIGNED_LONG, -1, -1, -1}},
	{124, "adjtimex", INT, 1, {STRUCT_TIMEX_P, -1, -1, -1, -1, -1}},
	{125, "mprotect", INT, 3, {VOID_P, SIZE_T, INT, -1, -1, -1}},
	{126, "sigprocmask", INT, 3, {INT, SIGSET_T_P, SIGSET_T_P, -1, -1, -1}},
	{127, "create_module", CADDR_T, 2, {CHAR_P, SIZE_T, -1, -1, -1, -1}},
	{128, "init_module", INT, 3, {VOID_P, UNSIGNED_LONG, CHAR_P, -1, -1,
		-1}},
	{129, "delete_module", INT, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{130, "get_kernel_syms", INT, 1, {STRUCT_KERNEL_SYM_P, -1, -1, -1, -1,
		-1}},
	{131, "quotactl", INT, 4, {INT, CHAR_P, INT, CADDR_T, -1, -1}},
	{132, "getpgid", PID_T, 1, {PID_T, -1, -1, -1, -1, -1}},
	{133, "fchdir", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{134, "bdflush", INT, 2, {INT, LONG_P, -1, -1, -1, -1}},
	{135, "sysfs", INT, 2, {INT, CHAR_P, -1, -1, -1, -1}},
	{136, "personality", INT, 1, {UNSIGNED_LONG, -1, -1, -1, -1, -1}},
	{137, "afs_syscall", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{138, "setfsuid", INT, 1, {UID_T, -1, -1, -1, -1, -1}},
	{139, "setfsgid", INT, 1, {UID_T, -1, -1, -1, -1, -1}},
	{140, "_llseek", INT, 5, {UNSIGNED_INT, UNSIGNED_LONG, UNSIGNED_LONG,
		LOFF_T_P, UNSIGNED_INT, -1}},
	{141, "getdents", INT, 3, {UNSIGNED_INT, STRUCT_LINUX_DIRENT_P,
		UNSIGNED_INT, -1, -1, -1}},
	{142, "_newselect", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{143, "flock", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{144, "msync", INT, 3, {VOID_P, SIZE_T, INT, -1, -1, -1}},
	{145, "readv", SSIZE_T, 3, {INT, STRUCT_IOVEC_P, INT, -1, -1, -1}},
	{146, "writev", SSIZE_T, 3, {INT, STRUCT_IOVEC_P, INT, -1, -1, -1}},
	{147, "getsid", PID_T, 1, {PID_T, -1, -1, -1, -1, -1}},
	{148, "fdatasync", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{149, "_sysctl", INT, 1, {STRUCT___SYSCTL_ARGS_P, -1, -1, -1, -1, -1}},
	{150, "mlock", INT, 2, {VOID_P, SIZE_T, -1, -1, -1, -1}},
	{151, "munlock", INT, 2, {VOID_P, SIZE_T, -1, -1, -1, -1}},
	{152, "mlockall", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{153, "munlockall", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{154, "sched_setparam", INT, 2, {PID_T, STRUCT_SCHED_PARAM_P, -1, -1,
		-1, -1}},
	{155, "sched_getparam", INT, 2, {PID_T, STRUCT_SCHED_PARAM_P, -1, -1,
		-1, -1}},
	{156, "sched_setscheduler", INT, 3, {PID_T, INT, STRUCT_SCHED_PARAM_P,
		-1, -1, -1}},
	{157, "sched_getscheduler", INT, 1, {PID_T, -1, -1, -1, -1, -1}},
	{158, "sched_yield", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{159, "sched_get_priority_max", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{160, "sched_get_priority_min", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{161, "sched_rr_get_interval", INT, 2, {PID_T, STRUCT_TIMESPEC_P, -1,
		-1, -1, -1}},
	{162, "nanosleep", INT, 2, {STRUCT_TIMESPEC_P, STRUCT_TIMESPEC_P, -1,
		-1, -1, -1}},
	{163, "mremap", VOID_P, 5, {VOID_P, SIZE_T, SIZE_T, INT, VARARGS, -1}},
	{164, "setresuid", INT, 3, {UID_T, UID_T, UID_T, -1, -1, -1}},
	{165, "getresuid", INT, 3, {UID_T_P, UID_T_P, UID_T_P, -1, -1, -1}},
	{166, "vm86", INT, 1, {STRUCT_VM86_STRUCT_P, -1, -1, -1, -1, -1}},
	{167, "query_module", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{168, "poll", INT, 3, {STRUCT_POLLFD_P, NFDS_T, INT, -1, -1, -1}},
	{169, "nfsservctl", LONG, 3, {INT, STRUCT_NFSCTL_ARG_P,
		UNION_NFSCTL_RES_P, -1, -1, -1}},
	{170, "setresgid", INT, 3, {GID_T, GID_T, GID_T, -1, -1, -1}},
	{171, "getresgid", INT, 3, {GID_T_P, GID_T_P, GID_T_P, -1, -1, -1}},
	{172, "prctl", INT, 5, {INT, UNSIGNED_LONG, UNSIGNED_LONG,
		UNSIGNED_LONG, UNSIGNED_LONG, -1}},
	{173, "rt_sigreturn", INT, 1, {UNSIGNED_LONG, -1, -1, -1, -1, -1}},
	{174, "rt_sigaction", INT, 3, {INT, STRUCT_SIGACTION_P,
		STRUCT_SIGACTION_P, -1, -1, -1}},
	{175, "rt_sigprocmask", INT, 3, {INT, SIGSET_T_P, SIGSET_T_P, -1, -1,
		-1}},
	{176, "rt_sigpending", INT, 1, {SIGSET_T_P, -1, -1, -1, -1, -1}},
	{177, "rt_sigtimedwait", INT, 3, {SIGSET_T_P, SIGINFO_T_P,
		STRUCT_TIMESPEC_P, -1, -1, -1}},
	{178, "rt_sigqueueinfo", INT, 3, {PID_T, INT, SIGINFO_T_P, -1, -1, -1}},
	{179, "rt_sigsuspend", INT, 1, {SIGSET_T_P, -1, -1, -1, -1, -1}},
	{180, "pread64", SSIZE_T, 4, {INT, VOID_P, SIZE_T, OFF_T, -1, -1}},
	{181, "pwrite64", SSIZE_T, 4, {INT, VOID_P, SIZE_T, OFF_T, -1, -1}},
	{182, "chown", INT, 3, {CHAR_P, UID_T, GID_T, -1, -1, -1}},
	{183, "getcwd", CHAR_P, 2, {CHAR_P, SIZE_T, -1, -1, -1, -1}},
	{184, "capget", INT, 2, {CAP_USER_HEADER_T, CAP_USER_DATA_T, -1, -1,
		-1, -1}},
	{185, "capset", INT, 2, {CAP_USER_HEADER_T, CAP_USER_DATA_T, -1, -1,
		-1, -1}},
	{186, "sigaltstack", INT, 2, {STACK_T_P, STACK_T_P, -1, -1, -1, -1}},
	{187, "sendfile", SSIZE_T, 4, {INT, INT, OFF_T_P, SIZE_T, -1, -1}},
	{188, "getpmsg", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{189, "putpmsg", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{190, "vfork", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{191, "ugetrlimit", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{192, "mmap2", VOID_P, 6, {VOID_P, SIZE_T, INT, INT, INT, OFF_T}},
	{193, "truncate64", INT, 2, {CHAR_P, OFF_T, -1, -1, -1, -1}},
	{194, "ftruncate64", INT, 2, {INT, OFF_T, -1, -1, -1, -1}},
	{195, "stat64", INT, 2, {CHAR_P, STRUCT_STAT_P, -1, -1, -1, -1}},
	{196, "lstat64", INT, 2, {CHAR_P, STRUCT_STAT_P, -1, -1, -1, -1}},
	{197, "fstat64", INT, 2, {INT, STRUCT_STAT_P, -1, -1, -1, -1}},
	{198, "lchown32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{199, "getuid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{200, "getgid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{201, "geteuid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{202, "getegid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{203, "setreuid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{204, "setregid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{205, "getgroups32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{206, "setgroups32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{207, "fchown32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{208, "setresuid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{209, "getresuid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{210, "setresgid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{211, "getresgid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{212, "chown32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{213, "setuid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{214, "setgid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{215, "setfsuid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{216, "setfsgid32", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{217, "pivot_root", INT, 2, {CHAR_P, CHAR_P, -1, -1, -1, -1}},
	{218, "mincore", INT, 3, {VOID_P, SIZE_T, UNSIGNED_CHAR_P, -1, -1, -1}},
	{219, "madvise", INT, 3, {VOID_P, SIZE_T, INT, -1, -1, -1}},
	{220, "getdents64", INT, 3, {UNSIGNED_INT, STRUCT_LINUX_DIRENT_P,
		UNSIGNED_INT, -1, -1, -1}},
	{221, "fcntl64", INT, 3, {INT, INT, VARARGS, -1, -1, -1}},
	{224, "gettid", PID_T, 1, {VOID, -1, -1, -1, -1, -1}},
	{225, "readahead", SSIZE_T, 3, {INT, OFF64_T, SIZE_T, -1, -1, -1}},
	{226, "setxattr", INT, 5, {CHAR_P, CHAR_P, VOID_P, SIZE_T, INT, -1}},
	{227, "lsetxattr", INT, 5, {CHAR_P, CHAR_P, VOID_P, SIZE_T, INT, -1}},
	{228, "fsetxattr", INT, 5, {INT, CHAR_P, VOID_P, SIZE_T, INT, -1}},
	{229, "getxattr", SSIZE_T, 4, {CHAR_P, CHAR_P, VOID_P, SIZE_T, -1, -1}},
	{230, "lgetxattr", SSIZE_T, 4, {CHAR_P, CHAR_P, VOID_P, SIZE_T, -1,
		-1}},
	{231, "fgetxattr", SSIZE_T, 4, {INT, CHAR_P, VOID_P, SIZE_T, -1, -1}},
	{232, "listxattr", SSIZE_T, 3, {CHAR_P, CHAR_P, SIZE_T, -1, -1, -1}},
	{233, "llistxattr", SSIZE_T, 3, {CHAR_P, CHAR_P, SIZE_T, -1, -1, -1}},
	{234, "flistxattr", SSIZE_T, 3, {INT, CHAR_P, SIZE_T, -1, -1, -1}},
	{235, "removexattr", INT, 2, {CHAR_P, CHAR_P, -1, -1, -1, -1}},
	{236, "lremovexattr", INT, 2, {CHAR_P, CHAR_P, -1, -1, -1, -1}},
	{237, "fremovexattr", INT, 2, {INT, CHAR_P, -1, -1, -1, -1}},
	{238, "tkill", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{239, "sendfile64", SSIZE_T, 4, {INT, INT, OFF_T_P, SIZE_T, -1, -1}},
	{240, "futex", INT, 6, {INT_P, INT, INT, STRUCT_TIMESPEC_P, INT_P,
		INT}},
	{241, "sched_setaffinity", INT, 3, {PID_T, SIZE_T, CPU_SET_T_P, -1, -1,
		-1}},
	{242, "sched_getaffinity", INT, 3, {PID_T, SIZE_T, CPU_SET_T_P, -1, -1,
		-1}},
	{243, "set_thread_area", INT, 1, {STRUCT_USER_DESC_P, -1, -1, -1, -1,
		-1}},
	{244, "get_thread_area", INT, 1, {STRUCT_USER_DESC_P, -1, -1, -1, -1,
		-1}},
	{245, "io_setup", INT, 2, {UNSIGNED_INT, AIO_CONTEXT_T_P, -1, -1,
		-1, -1}},
	{246, "io_destroy", INT, 1, {AIO_CONTEXT_T, -1, -1, -1, -1, -1}},
	{247, "io_getevents", INT, 5, {AIO_CONTEXT_T, LONG, LONG,
		STRUCT_IO_EVENT_P, STRUCT_TIMESPEC_P, -1}},
	{248, "io_submit", INT, 3, {AIO_CONTEXT_T, LONG, STRUCT_IOCB_PP, -1,
		-1, -1}},
	{249, "io_cancel", INT, 3, {AIO_CONTEXT_T, STRUCT_IOCB_P,
		STRUCT_IO_EVENT_P, -1, -1, -1}},
	{250, "fadvise64", INT, 4, {INT, OFF_T, OFF_T, INT, -1, -1}},
	{252, "exit_group", VOID, 1, {INT, -1, -1, -1, -1, -1}},
	{253, "lookup_dcookie", INT, 3, {U64, CHAR_P, SIZE_T, -1, -1, -1}},
	{254, "epoll_create", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{255, "epoll_ctl", INT, 4, {INT, INT, INT, STRUCT_EPOLL_EVENT_P, -1,
		-1}},
	{256, "epoll_wait", INT, 4, {INT, STRUCT_EPOLL_EVENT_P, INT, INT, -1,
		-1}},
	{257, "remap_file_pages", INT, 5, {VOID_P, SIZE_T, INT, SSIZE_T, INT,
		-1}},
	{258, "set_tid_address", LONG, 1, {INT_P, -1, -1, -1, -1, -1}},
	{259, "timer_create", INT, 3, {CLOCKID_T, STRUCT_SIGEVENT_P, TIMER_T_P,
		-1, -1, -1}},
	{260, "timer_settime", INT, 4, {TIMER_T, INT, STRUCT_ITIMERSPEC_P,
		STRUCT_ITIMERSPEC_P, -1, -1}},
	{261, "timer_gettime", INT, 2, {TIMER_T, STRUCT_ITIMERSPEC_P, -1, -1,
		-1, -1}},
	{262, "timer_getoverrun", INT, 1, {TIMER_T, -1, -1, -1, -1, -1}},
	{263, "timer_delete", INT, 1, {TIMER_T, -1, -1, -1, -1, -1}},
	{264, "clock_settime", INT, 2, {CLOCKID_T, STRUCT_TIMESPEC_P, -1, -1,
		-1, -1}},
	{265, "clock_gettime", INT, 2, {CLOCKID_T, STRUCT_TIMESPEC_P, -1, -1,
		-1, -1}},
	{266, "clock_getres", INT, 2, {CLOCKID_T, STRUCT_TIMESPEC_P, -1, -1,
		-1, -1}},
	{267, "clock_nanosleep", INT, 4, {CLOCKID_T, INT, STRUCT_TIMESPEC_P,
		STRUCT_TIMESPEC_P, -1, -1}},
	{268, "statfs64", INT, 2, {CHAR_P, STRUCT_STATFS_P, -1, -1, -1, -1}},
	{269, "fstatfs64", INT, 2, {INT, STRUCT_STATFS_P, -1, -1, -1, -1}},
	{270, "tgkill", INT, 3, {INT, INT, INT, -1, -1, -1}},
	{271, "utimes", INT, 2, {CHAR_P, STRUCT_TIMEVAL_P, -1, -1, -1, -1}},
	{272, "fadvise64_64", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{273, "vserver", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{274, "mbind", INT, 6, {VOID_P, UNSIGNED_LONG, INT, UNSIGNED_LONG_P,
		UNSIGNED_LONG, UNSIGNED_INT}},
	{275, "get_mempolicy", INT, 5, {INT_P, UNSIGNED_LONG_P, UNSIGNED_LONG,
		UNSIGNED_LONG, UNSIGNED_LONG, -1}},
	{276, "set_mempolicy", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{277, "mq_open", MQD_T, 2, {CHAR_P, INT, -1, -1, -1, -1}},
	{278, "mq_unlink", INT, 1, {CHAR_P, -1, -1, -1, -1, -1}},
	{279, "mq_timedsend", INT, 5, {MQD_T, CHAR_P, SIZE_T,
		UNSIGNED_INT, STRUCT_TIMESPEC_P, -1}},
	{280, "mq_timedreceive", SSIZE_T, 5, {MQD_T, CHAR_P, SIZE_T,
		UNSIGNED_INT_P, STRUCT_TIMESPEC_P, -1}},
	{281, "mq_notify", INT, 2, {MQD_T, STRUCT_SIGEVENT_P, -1, -1, -1, -1}},
	{282, "mq_getsetattr", INT, 3, {MQD_T, STRUCT_MQ_ATTR_P,
		STRUCT_MQ_ATTR_P, -1, -1, -1}},
	{283, "kexec_load", LONG, 4, {UNSIGNED_LONG, UNSIGNED_LONG,
		STRUCT_KEXEC_SEGMENT_P, UNSIGNED_LONG, -1, -1}},
	{284, "waitid", INT, 4, {IDTYPE_T, ID_T, SIGINFO_T_P, INT, -1, -1}},
	{286, "add_key", KEY_SERIAL_T, 5, {CHAR_P, CHAR_P, VOID_P, SIZE_T,
		KEY_SERIAL_T, -1}},
	{287, "request_key", KEY_SERIAL_T, 4, {CHAR_P, CHAR_P, CHAR_P,
		KEY_SERIAL_T, -1, -1}},
	{288, "keyctl", LONG, 2, {INT, VARARGS, -1, -1, -1, -1}},
	{289, "ioprio_set", INT, 3, {INT, INT, INT, -1, -1, -1}},
	{290, "ioprio_get", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{291, "inotify_init", INT, 1, {VOID, -1, -1, -1, -1, -1}},
	{292, "inotify_add_watch", INT, 3, {INT, CHAR_P, UINT32_T, -1, -1, -1}},
	{293, "inotify_rm_watch", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{294, "migrate_pages", LONG, 4, {INT, UNSIGNED_LONG, UNSIGNED_LONG_P,
		UNSIGNED_LONG_P, -1, -1}},
	{295, "openat", INT, 3, {INT, CHAR_P, INT, -1, -1, -1}},
	{296, "mkdirat", INT, 3, {INT, CHAR_P, MODE_T, -1, -1, -1}},
	{297, "mknodat", INT, 4, {INT, CHAR_P, MODE_T, DEV_T, -1, -1}},
	{298, "fchownat", INT, 5, {INT, CHAR_P, UID_T, GID_T, INT, -1}},
	{299, "futimesat", INT, 3, {INT, CHAR_P, STRUCT_TIMEVAL_P, -1, -1, -1}},
	{300, "fstatat64", INT, 4, {INT, CHAR_P, STRUCT_STAT_P, INT, -1, -1}},
	{301, "unlinkat", INT, 3, {INT, CHAR_P, INT, -1, -1, -1}},
	{302, "renameat", INT, 4, {INT, CHAR_P, INT, CHAR_P, -1, -1}},
	{303, "linkat", INT, 5, {INT, CHAR_P, INT, CHAR_P, INT, -1}},
	{304, "symlinkat", INT, 3, {CHAR_P, INT, CHAR_P, -1, -1, -1}},
	{305, "readlinkat", INT, 4, {INT, CHAR_P, CHAR_P, SIZE_T, -1, -1}},
	{306, "fchmodat", INT, 4, {INT, CHAR_P, MODE_T, INT, -1, -1}},
	{307, "faccessat", INT, 4, {INT, CHAR_P, INT, INT, -1, -1}},
	{308, "pselect6", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{309, "ppoll", INT, 4, {STRUCT_POLLFD_P, NFDS_T, STRUCT_TIMESPEC_P,
		SIGSET_T_P, -1, -1}},
	{310, "unshare", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{311, "set_robust_list", LONG, 2, {STRUCT_ROBUST_LIST_HEAD_P, SIZE_T,
		-1, -1, -1, -1}},
	{312, "get_robust_list", LONG, 3, {INT, STRUCT_ROBUST_LIST_HEAD_PP,
		SIZE_T_P, -1, -1, -1}},
	{313, "splice", SSIZE_T, 6, {INT, LOFF_T_P, INT, LOFF_T_P, SIZE_T,
		UNSIGNED_INT}},
	{314, "sync_file_range", INT, 4, {INT, OFF64_T, OFF64_T, UNSIGNED_INT,
		-1, -1}},
	{315, "tee", SSIZE_T, 4, {INT, INT, SIZE_T, UNSIGNED_INT, -1, -1}},
	{316, "vmsplice", SSIZE_T, 4, {INT, STRUCT_IOVEC_P, UNSIGNED_LONG,
		UNSIGNED_INT, -1, -1}},
	{317, "move_pages", LONG, 6, {INT, UNSIGNED_LONG, VOID_PP, INT_P,
		INT_P, INT}},
	{318, "getcpu", INT, 3, {UNSIGNED_INT_P, UNSIGNED_INT_P,
		STRUCT_GETCPU_CACHE_P, -1, -1, -1}},
	{319, "epoll_pwait", INT, 5, {INT, STRUCT_EPOLL_EVENT_P, INT, INT,
		SIGSET_T_P, -1}},
	{320, "utimensat", INT, 4, {INT, CHAR_P, STRUCT_TIMESPEC_P, INT,
		-1, -1}},
	{321, "signalfd", INT, 3, {INT, SIGSET_T_P, INT, -1, -1, -1}},
	{322, "timerfd_create", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{323, "eventfd", INT, 2, {UNSIGNED_INT, INT, -1, -1, -1, -1}},
	{324, "fallocate", INT, 4, {INT, INT, OFF_T, OFF_T, -1, -1}},
	{325, "timerfd_settime", INT, 4, {INT, INT, STRUCT_ITIMERSPEC_P,
		STRUCT_ITIMERSPEC_P, -1, -1}},
	{326, "timerfd_gettime", INT, 2, {INT, STRUCT_ITIMERSPEC_P, -1, -1, -1,
		-1}},
	{327, "signalfd4", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{328, "eventfd2", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{329, "epoll_create1", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{330, "dup3", INT, 3, {INT, INT, INT, -1, -1, -1}},
	{331, "pipe2", INT, 2, {INT_P, INT, -1, -1, -1, -1}},
	{332, "inotify_init1", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{333, "preadv", SSIZE_T, 4, {INT, STRUCT_IOVEC_P, INT, OFF_T, -1, -1}},
	{334, "pwritev", SSIZE_T, 4, {INT, STRUCT_IOVEC_P, INT, OFF_T, -1, -1}},
	{335, "rt_tgsigqueueinfo", INT, 4, {PID_T, PID_T, INT, SIGINFO_T_P, -1,
		-1}},
	{336, "perf_event_open", INT, 5, {STRUCT_PERF_EVENT_ATTR_P, PID_T, INT,
		INT, UNSIGNED_LONG, -1}},
	{337, "recvmmsg", INT, 5, {INT, STRUCT_MMSGHDR_P, UNSIGNED_INT,
		UNSIGNED_INT, STRUCT_TIMESPEC_P, -1}},
	{338, "fanotify_init", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{339, "fanotify_mark", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{340, "prlimit64", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{341, "name_to_handle_at", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{342, "open_by_handle_at", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{343, "clock_adjtime", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{344, "syncfs", INT, 1, {INT, -1, -1, -1, -1, -1}},
	{345, "sendmmsg", INT, 4, {INT, STRUCT_MMSGHDR_P, UNSIGNED_INT,
		UNSIGNED_INT, -1, -1}},
	{346, "setns", INT, 2, {INT, INT, -1, -1, -1, -1}},
	{347, "process_vm_readv", SSIZE_T, 6, {PID_T, STRUCT_IOVEC_P,
		UNSIGNED_LONG, STRUCT_IOVEC_P, UNSIGNED_LONG, UNSIGNED_LONG}},
	{348, "process_vm_writev", SSIZE_T, 6, {PID_T, STRUCT_IOVEC_P,
		UNSIGNED_LONG, STRUCT_IOVEC_P, UNSIGNED_LONG, UNSIGNED_LONG}},
	{349, "kcmp", INT, 5, {PID_T, PID_T, INT, UNSIGNED_LONG, UNSIGNED_LONG,
		-1}},
	{350, "finit_module", INT, 3, {INT, CHAR_P, INT, -1, -1, -1}},
	{351, "sched_setattr", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{352, "sched_getattr", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{353, "renameat2", -1, 0, {-1, -1, -1, -1, -1, -1}},
	{354, "seccomp", -1, 0, {-1, -1, -1, -1, -1, -1}}
};


#endif /* !_SYSCALLS_H_ */
