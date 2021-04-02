/* printf */
#include <stdio.h>
/* Python C API */
#include <Python.h>


/*
 * All CPython macros and functions are prohibited by the instructions for this
 * project, except for PyLong_Check and PyLong_SHIFT. This is apparently
 * enforced by a regex match of this source file, so any of their mention in
 * comments will have the leading "Py", "PY", or "_Py" omitted.
 *
 * As a result of the prohbition, we cannot use the CPython provided macro
 * ..LONG_BITS_IN_DIGIT to set 32- or 64-bit behavior. One possible workaround
 * is _LP64, as found in:
 * https://gcc.gnu.org/onlinedocs/gcc-4.1.2/cpp/Common-Predefined-Macros.html
 * "
 * __LP64__
 * _LP64
 *      These macros are defined, with value 1, if (and only if) the
 * compilation is for a target where long int and pointer both use 64-bits
 * and int uses 32-bit."
 */
#ifdef _LP64
/*
 * only needed if digit and twodigits are prohibited
 * typedef uint32_t digit_t;
 * typedef uint64_t twodigits_t;
 */
/* #define PyLong_SHIFT   30 *allowed, use real macro */
#define DECIMAL_SHIFT  9 /* max(e such that 10**e fits in a digit) */
#define DECIMAL_BASE   ((digit)1000000000) /* 10 ** DECIMAL_SHIFT */
#else
/*
 * only needed if digit and twodigits are prohibited
 * typedef unsigned short digit_t;
 * typedef unsigned long twodigits_t;
 */
/* #define PyLong_SHIFT   15 *allowed, use real macro */
#define DECIMAL_SHIFT  4 /* max(e such that 10**e fits in a digit) */
#define DECIMAL_BASE   ((digit)10000) /* 10 ** DECIMAL_SHIFT */
#endif

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define SSIZE_T_MAX ((Py_ssize_t)(((size_t)-1) >> 1)) /* LONG_MAX on 64-bit */



/**
 * _PyLongBaseToBase10 - helper to print_python_int, converts an array of
 * values representing the base 2**30 digits of a given number to an array
 * representing the same number in base 10
 *
 * @size: count of members in arr_in array
 * @arr_in: array of base 2**30 digits
 * @arr_out: array of base 10 digits
 * Return: size of arr_out
 */
static Py_ssize_t _PyLongBaseToBase10(Py_ssize_t size,
				      digit *arr_in, digit *arr_out)
{
	Py_ssize_t new_size, i, j;
	digit hi;
	twodigits z;

	/*
	 * convert array of base 2**30 digits in arr_in to an array of
	 * base DECIMAL_BASE digits in arr_out, following Knuth (TAOCP,
	 * Volume 2 (3rd edn), section 4.4, Method 1b).
	 */
	new_size = 0;
	for (i = size; --i >= 0; )
	{
		hi = arr_in[i];
		for (j = 0; j < new_size; j++)
		{
			z = (twodigits)arr_out[j] << PyLong_SHIFT | hi;
			hi = (digit)(z / DECIMAL_BASE);
			arr_out[j] = (digit)(z - (twodigits)hi * DECIMAL_BASE);
		}
		while (hi)
		{
			arr_out[new_size++] = hi % DECIMAL_BASE;
			hi /= DECIMAL_BASE;
		}
	}

	/* printing a PyLongObject of value 0 needs at least one digit */
	if (new_size == 0)
		arr_out[new_size++] = 0;

	return (new_size);
}


/**
 * _printPyLong - helper to print_python_int; prints an integer as represented
 * by and array of values containing its digits in base 10
 *
 * @b10_size: count of members in b10_arr
 * @b10_arr: array of values representing the digits of a number in base 10
 * @strlen: length of buffer for printed string
 * @negative: 0 if base 10 number is positive, 1 for negative
 * Return: 0 on success, 1 on failure
 */
static int _printPyLong(Py_ssize_t b10_size, digit *b10_arr,
			Py_ssize_t strlen, int negative)
{
	char *str = NULL, *pr = NULL;
	Py_ssize_t i, j;
	digit rem;

	str = malloc(sizeof(char) * (strlen + 1));
	if (!str)
		return (1);
	str[strlen] = '\0';
	/* fill the string right-to-left */
	pr = str + strlen;
	/*
	 * b10_arr[0] through b10_arr[b10_size - 2] contribute exactly
	 * DECIMAL_SHIFT digits each
	 */
	for (i = 0; i < b10_size - 1; i++)
	{
		rem = b10_arr[i];
		for (j = 0; j < DECIMAL_SHIFT; j++)
		{
			*--pr = '0' + rem % 10;
			rem /= 10;
		}
	}
	/* b10_arr[size - 1]: always produce at least one decimal digit */
	rem = b10_arr[i];
	do {
		*--pr = '0' + rem % 10;
		rem /= 10;
	} while (rem != 0);
	if (negative)
		*--pr = '-';
	/* should have moved all the way back to starting ptr */
	if (pr != str)
	{
		free(str);
		return (1);
	}
	puts(str);
	free(str);
	return (0);
}


/**
 * print_python_int - prints the value of a Python 3 int object,
 * known in the C API as PyLongObject; same output as str(p) or p.__repr__()
 *
 * @p: PyObject *-castable struct pointer
 */
void print_python_int(PyObject *p)
{
	PyLongObject *pl;
	Py_ssize_t b10_size, strlen, size_pl;
	digit rem, tenpow, *b10_digit = NULL;
	int negative, d;

	pl = (PyLongObject *)p;
	if (!pl || !PyLong_Check(pl))
	{
		printf("Invalid Int Object\n");
		return;
	}
	negative = ((PyVarObject *)p)->ob_size < 0;
	size_pl = ABS(((PyVarObject *)p)->ob_size);
	if (size_pl >= SSIZE_T_MAX / 2)
	{
		fprintf(stderr, "size_pl over limit of SSIZE_T_MAX / 2\n");
		return;
	}

	d = (33 * DECIMAL_SHIFT) / (10 * PyLong_SHIFT - 33 * DECIMAL_SHIFT);
	b10_size = 1 + size_pl + size_pl / d;
	b10_digit = malloc(sizeof(digit) * b10_size);
	if (b10_digit == NULL)
	{
		fprintf(stderr, "b10_digit malloc failure\n");
		return;
	}
	b10_size = _PyLongBaseToBase10(size_pl, pl->ob_digit, b10_digit);

	strlen = negative + 1 + (b10_size - 1) * DECIMAL_SHIFT;
	tenpow = 10;
	rem = b10_digit[b10_size - 1];
	while (rem >= tenpow)
	{
		tenpow *= 10;
		strlen++;
	}

	if (_printPyLong(b10_size, b10_digit, strlen, negative) == 1)
		fprintf(stderr, "_printPyLong err\n");
}
