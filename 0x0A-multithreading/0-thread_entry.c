#include "multithreading.h"
/* pthread_exit */
#include <pthread.h>
/* puts */
#include <stdio.h>


/**
 * thread_entry - entry point to a new thread
 *
 * @arg: address of a string that must be printed followed by '\n'
 * Return: NULL by default, void pointer could be used for modified struct
 *   coming in via `arg`
 */
void *thread_entry(void *arg)
{
	if (!arg)
		pthread_exit(NULL);

	puts((char *)arg);

	pthread_exit(NULL);
}
