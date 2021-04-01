/* printf */
#include <stdio.h>
/* Python C API */
#include <Python.h>

/* bool true false */
#include <stdbool.h>


/*
/usr/include/python3.4/pyport.h:96:#define PY_UINT32_T uint32_t
/usr/include/python3.4/pyport.h:109:#define PY_UINT64_T uint64_t
/usr/include/python3.4m/longintrepr.h-49-typedef PY_UINT32_T digit;
/usr/include/python3.4m/longintrepr.h-51-typedef PY_UINT64_T twodigits;
*/

#define ABS(x) ((x) < 0 ? -(x) : (x)) /* Py_ABS */
/* assuming PYLONG_BITS_IN_DIGIT == 30 for digit/DECIMAL/SHIFT macros */
/* digit = uint32_t */
/* twodigit = unit64_t */
#define DECIMAL_SHIFT 9 /* max(e such that 10**e fits in a digit) */ /*_PyLong_DECIMAL_SHIFT */
#define DECIMAL_BASE  ((digit)1000000000) /* 10 ** DECIMAL_SHIFT, digit == uint32_t */ /* _PyLong_DECIMAL_BASE */
/* #define PyLong_SHIFT   30 */ /* allowed by assignment, no analog */
#define SSIZE_T_MAX ((Py_ssize_t)LONG_MAX) /* PY_SSIZE_T_MAX ((Py_ssize_t)(((size_t)-1)>>1)) */ /* LONG_MAX on my machine */

static Py_ssize_t _PyLongBaseToBase10(Py_ssize_t size_pl, digit *pin, digit *pout)
{
	Py_ssize_t b10_size, i, j;
	digit hi;
	twodigits z;

	/* convert array of base 2**30 digits in pin to an array of
	   base DECIMAL_BASE digits in pout, following Knuth (TAOCP,
	   Volume 2 (3rd edn), section 4.4, Method 1b). */
	b10_size = 0;
	for (i = size_pl; --i >= 0; )
	{
		/* hi is digit, or uint32_t */
		hi = pin[i];
		for (j = 0; j < b10_size; j++)
		{
			/* z is twodigits, or uint64_t */
			z = (twodigits)pout[j] << PyLong_SHIFT | hi;
			hi = (digit)(z / DECIMAL_BASE);
			pout[j] = (digit)(z - (twodigits)hi * DECIMAL_BASE);
		}
		while (hi)
		{
			pout[b10_size++] = hi % DECIMAL_BASE;
			hi /= DECIMAL_BASE;
		}
		/* check for keyboard interrupt with sighandler? */
	}
	/* pout should have at least one digit, so that the case when a = 0
	   works correctly */
	if (b10_size == 0)
		pout[b10_size++] = 0;

	return (b10_size);
}



static int _printPyLong(Py_ssize_t strlen, Py_ssize_t b10_size, digit *pout, int negative)
{
	char *str = NULL, *pr = NULL;
	Py_ssize_t i, j;
	digit rem;

	str = malloc(sizeof(char) * (strlen + 1));
	if (!str)
		/* print err? */
		return (1);
	str[strlen] = '\0';

	/* fill the string right-to-left */
        pr = str + strlen;
	/* pout[0] through pout[b10_size - 2] contribute exactly
	   DECIMAL_SHIFT digits each */
	for (i = 0; i < b10_size - 1; i++)
	{
		rem = pout[i];
		for (j = 0; j < DECIMAL_SHIFT; j++)
		{
			*--pr = '0' + rem % 10;
			rem /= 10;
		}
	}
	/* pout[size - 1]: always produce at least one decimal digit */
	rem = pout[i];
	do {
		*--pr = '0' + rem % 10;
		rem /= 10;
	} while (rem != 0);
	/* and sign */
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
	return(0);
}



/**
 * print_python_int - 
 *  of a Python 3 int object,
 * known in the C API as a Long object
 *
 * @p: PyObject *-castable struct pointer
 */
void print_python_int(PyObject *p)
{
	PyLongObject *pl;
	Py_ssize_t b10_size, strlen, size_pl, i;
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

	printf("  ob_size: %li\n", size_pl);

	for (i = 0; i < ABS(size_pl); i++)
	{
	        if (i == 0)
			printf("  LSD first in base 2**30: [ ");
		printf("%i, ", pl->ob_digit[i]);
		if (i == ABS(size_pl) - 1)
			printf("]\n");

	}

	/* operator precedence means this is equal to ?:
	d = (33 * DECIMAL_SHIFT) /
		((10 * PyLong_SHIFT) - (33 * DECIMAL_SHIFT));
	*/
	d = (33 * DECIMAL_SHIFT) / (10 * PyLong_SHIFT - 33 * DECIMAL_SHIFT);
/*
	printf("\n\td = (33 * DECIMAL_SHIFT) / (10 * PyLong_SHIFT - 33 * DECIMAL_SHIFT);\n");
	printf("\t%i = (33 * %i) / (10 * %i - 33 * %i);\n",
	       d, DECIMAL_SHIFT, PyLong_SHIFT, DECIMAL_SHIFT);
*/
	if (size_pl >= SSIZE_T_MAX / 2)
	{
		fprintf(stderr, "size_pl over limit of SSIZE_T_MAX / 2:%lu\n", SSIZE_T_MAX / 2);
		return;
	}

	b10_size = 1 + size_pl + size_pl / d;
/*
	printf("\n\tb10_size = 1 + size_pl + size_pl / d;\n");
	printf("\t%li = 1 + %li + %li / %i;\n", b10_size, size_pl, size_pl, d);
*/
	b10_digit = malloc(sizeof(digit) * b10_size);
/*
	printf("\n\tb10_digit = malloc(sizeof(digit) * b10_size);\n");
	printf("\t%p = malloc(%lu * %li);\n", (void *)b10_digit, sizeof(digit), b10_size);
*/
	if (b10_digit == NULL)
	{
		fprintf(stderr, "b10_digit malloc failure\n");
		return;
	}

	b10_size = _PyLongBaseToBase10(size_pl, pl->ob_digit, b10_digit);
/*
	printf("\n\tb10_size = _PyLongBaseToBase10(size_pl, pl->ob_digit, b10_digit);\n");
	printf("\t%li = _PyLongBaseToBase10(%li, %p, %p);\n", b10_size, size_pl, (void *)(pl->ob_digit), (void *)b10_digit);
*/
	/* calculate length of output string */
	strlen = negative + 1 + (b10_size - 1) * DECIMAL_SHIFT;
	tenpow = 10;
	rem = b10_digit[b10_size - 1];
	while (rem >= tenpow)
	{
		tenpow *= 10;
		strlen++;
	}
	printf("\tstrlen:%li\n", strlen);

	if (_printPyLong(strlen, b10_size, b10_digit, negative) == 1)
		fprintf(stderr, "_printPyLong err\n");
}
