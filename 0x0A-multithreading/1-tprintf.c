#include "multithreading.h"
/* pthread_self */
#include <pthread.h>
/* va_list va_start va_end */
#include <stdarg.h>
/* printf vprintf putchar */
#include <stdio.h>


/**
 * tprintf - uses the printf family to print out a given formatted string
 *
 * @format: format string for printed output, followed by variable args
 * Return: number of bytes printed from format, or -1 on failure
 */
int tprintf(char const *format, ...)
{
	va_list valist;
	int msg_bytes = 0;

	if (!format)
		return (-1);

	/* man 3 pthread_self: "This function always succeeds." */
	printf("[%lu] ", pthread_self());

	va_start(valist, format);
	msg_bytes = vprintf(format, valist);
	va_end(valist);

	if (msg_bytes == -1)
		putchar('\n');

	return (msg_bytes);
}
