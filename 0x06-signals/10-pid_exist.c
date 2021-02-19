#include <sys/types.h>
#include <signal.h>

/**
 * pid_exist - tests if a process exists, given its PID
 * note 1: kill with signum of 0 does error checking without sending signal
 * note 2: project limitations on includes and length prevent use of errno
 * note 3: project main expects 0 for PID not found, thus +1 to invert return
 *
 * @pid: process ID to test
 * Return: 0 if process not found, 1 if found
 */
int pid_exist(pid_t pid)
{
	return (kill(pid, 0) + 1);
}
