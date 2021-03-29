/* printf */
#include <stdio.h>
/* Python C API */
#include <Python.h>


/**
 * print_python_bytes - prints member count, allocated spaces, and member
 * element types for python lists
 *
 * Macros/functions prohibited by project task instructions, and their
 * respective definitions in Python3.4-dev or cpython source v3.10:
 *
 *  Py_SIZE(ob) - (((PyVarObject *)(ob))->ob_size)
 *  Py_TYPE(ob) - (((PyObject *)(ob))->ob_type)
 *  PyList_GetItem - ((PyListObject *)op)->ob_item[i] + error checking
 *  PyBytes_AS_STRING(op) - (assert(PyBytes_Check(op)),
 * (((PyBytesObject *)(op))->ob_sval))
 *  PyBytes_GET_SIZE - (assert(PyBytes_Check(op)), Py_SIZE(op))
 *
 * @p: PyObject *-castable struct pointer
 */
void print_python_bytes(PyObject *p)
{
	int i;
	Py_ssize_t size, printed_bytes;
	char *array_as_string = NULL;

	printf("[.] bytes object info\n");
	if (!PyBytes_Check(p))
	{
		printf("  [ERROR] Invalid Bytes Object\n");
		return;
	}

	size = ((PyVarObject *)(p))->ob_size;
	printf("  size: %li\n", size);

	array_as_string = ((PyBytesObject *)(p))->ob_sval;
	printf("  trying string: %s\n", array_as_string);

	printed_bytes = (size + 1 >= 10) ? 10 : size + 1;
	printf("  first %li bytes:", printed_bytes);
	for (i = 0; i < printed_bytes; i++)
		printf(" %02x", (unsigned char)(array_as_string[i]));
	putchar('\n');
}


/**
 * print_python_list - prints member count, allocated spaces, and member
 * element types for python lists
 *
 * Macros/functions prohibited by project task instructions, and their
 * respective definitions in Python3.4-dev or cpython source v3.10:
 *
 *  Py_SIZE(ob) - (((PyVarObject*)(ob))->ob_size)
 *  Py_TYPE(ob) - (((PyObject*)(ob))->ob_type)
 *  PyList_GetItem - ((PyListObject *)op)->ob_item[i] + error checking
 *  PyBytes_AS_STRING(op) - (assert(PyBytes_Check(op)),
 * (((PyBytesObject *)(op))->ob_sval))
 *  PyBytes_GET_SIZE - (assert(PyBytes_Check(op)), Py_SIZE(op))
 *
 * @p: PyObject *-castable struct pointer
 */
void print_python_list(PyObject *p)
{
	int i;
	Py_ssize_t size;
	PyObject *list_member;

	printf("[*] Python list info\n");
	if (!PyList_Check(p))
	{
		printf("  [ERROR] Invalid List Object\n");
		return;
	}

	size = ((PyVarObject *)(p))->ob_size;
	printf("[*] Size of the Python List = %li\n", size);

	printf("[*] Allocated = %lu\n", ((PyListObject *)p)->allocated);

	for (i = 0; i < size; i++)
	{
		list_member = ((PyListObject *)p)->ob_item[i];
		printf("Element %d: %s\n", i,
		       list_member->ob_type->tp_name);

		if (PyBytes_Check(list_member))
			print_python_bytes(list_member);
	}
}
