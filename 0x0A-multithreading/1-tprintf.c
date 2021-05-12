#include "multithreading.h"
/* pthread_t pthread_self */
#include <pthread.h>
/* va_list */
#include <stdarg.h>
/* printf vprintf */
#include <stdio.h>


/**
 * tprintf - uses the printf family to print out a given formatted string
 *
 * @format: format string for printed output, followed by vargs
 * Return: number of bytes printed or 0 on failure
 */
int tprintf(char const *format, ...)
{
	va_list valist;
	int id_bytes = 0, msg_bytes = 0;

	/* man 3 pthread_self: "This function always succeeds." */
	id_bytes = printf("[%lu] ", pthread_self());

	va_start(valist, format);
	msg_bytes = vprintf(format, valist);
	va_end(valist);

	if (msg_bytes == -1)
	{
		putchar('\n');
		msg_bytes = 0;
	}

	return (id_bytes + msg_bytes);
}
