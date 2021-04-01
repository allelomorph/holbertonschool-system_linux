/* printf */
#include <stdio.h>
/* Python C API */
#include <Python.h>
/* bool true false */
#include <stdbool.h>

#define _ABS(x) ((x) < 0 ? -(x) : (x))

/**
 * print_python_int - prints the value of a Python 3 int object with an
 * absolute value less than ULONG_MAX
 *
 * @p: PyObject *-castable struct pointer
 */
void print_python_int(PyObject *p)
{
	Py_ssize_t i, size;
	bool negative;
	unsigned long base10 = 0, pow_base_pylong;
	/* base 1,073,741,824 or 2**30 or 2**PyLong_SHIFT */
	/* PyLong_SHIFT is 15 in 32-bit environment, 30 in 64 */
	unsigned int base_pylong = 1 << PyLong_SHIFT;

	if (!PyLong_Check(p))
	{
		puts("Invalid Int Object");
		return;
	}

	size = _ABS(((PyVarObject *)p)->ob_size);
	negative = ((PyVarObject *)p)->ob_size < 0;

	if (size < 3 ||
	    (size == 3 && ((PyLongObject *)p)->ob_digit[2] < 16))
	{
		pow_base_pylong = 1;
		for (i = 0; i < size; i++)
		{
			base10 += pow_base_pylong *
				((PyLongObject *)p)->ob_digit[i];
			pow_base_pylong *= base_pylong;
		}

		if (negative)
			putchar('-');
		printf("%lu\n", base10);
	}
	else
		puts("C unsigned long int overflow");
}
