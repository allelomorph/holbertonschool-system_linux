/* printf fflush */
#include <stdio.h>
/* Python C API */
#include <Python.h>
/* wchar_t */
#include <wchar.h>
/* setlocale */
#include <locale.h>


/**
 * print_python_string - prints the implmentation type, length (in bytes if
 * ASCII, or in characters if Unicode) and value of a Python 3 str object,
 * known in the C API as an Unicode object
 *
 * Note: not portable to C90, must be compiled in C99 standard
 *
 * @p: PyObject *-castable struct pointer
 */
void print_python_string(PyObject *p)
{
	wchar_t *wchar_buf = NULL;

	printf("[.] string object info\n");

	if (!PyUnicode_Check(p) || !((PyASCIIObject *)p)->state.ready)
	{
		printf("  [ERROR] Invalid String Object\n");
		return;
	}

	printf("  type: %s%s\n",
	       ((PyASCIIObject *)p)->state.compact ? "compact " : "",
	       ((PyASCIIObject *)p)->state.ascii ? "ascii" : "unicode object");

	printf("  length: %li\n",
	       ((PyASCIIObject *)(p))->length);

	/*
	 * When printing utf8/wchar_t in C, locale must be set beforehand.
	 * In this case is the locale already set by Python?
	 *
	 * setlocale(LC_ALL, "");
	 */

	wchar_buf = PyUnicode_AsWideCharString(p, NULL);
	if (wchar_buf != NULL)
	{
		/* printf %ls, %lc not in c90, only c99 */
		printf("  value: %ls\n", wchar_buf);

		PyMem_Free(wchar_buf);
	}
}
