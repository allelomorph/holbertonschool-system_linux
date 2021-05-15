#include "multithreading.h"
/* pthread_self pthread_mutex_init pthread_mutex_lock pthread_mutex_unlock */
#include <pthread.h>
/* va_list va_start va_end */
#include <stdarg.h>
/* vprintf putchar */
#include <stdio.h>


static pthread_mutex_t printf_mutex;

/**
 * initTprintfMutex - initializes global mutex for printf buffer before main
 */
void initTprintfMutex(void)
{
	pthread_mutex_init(&printf_mutex, NULL);
}


/**
 * destroyTprintfMutex - destroys global mutex for printf buffer after main
 */
void destroyTprintfMutex(void)
{
	pthread_mutex_destroy(&printf_mutex);
}


/**
 * tprintf - uses the printf family to print out a given formatted string
 *
 * @format: format string for printed output, followed by vargs
 * Return: number of bytes printed from format, or -1 on failure
 */
int tprintf(char const *format, ...)
{
	va_list args;
	int msg_bytes = 0;

	if (!format)
		return (-1);

	va_start(args, format);

	pthread_mutex_lock(&printf_mutex);

	printf("[%lu] ", pthread_self());
	msg_bytes = vprintf(format, args);

	pthread_mutex_unlock(&printf_mutex);

	va_end(args);

	return (msg_bytes);
}
