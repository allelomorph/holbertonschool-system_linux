#include <stdio.h>
#include <Python.h>

/**
 * print_python_list - prints member count, allocated spaces, and member
 * element types for python lists
 *
 * Notes for learners of Python C API:
 * - Below the same PyObject* pointer is cast to both PyVarObject* and
 * PyListObject*
 * - PyObject* is a generic pointer type to which all pointers to Python
 * objects can be cast
 * - Likewise, pointers to variable-size Python objects can be cast to
 * PyVarObject*
 * - PyVarObject.ob_size (via Py_SIZE) represents the number of elements in
 * a variable object; value returned by len()
 * - While PyListObject.allocated indicates the amount of members that can fit
 * into its .ob_item vector; should be <= ob_size
 * - Py_TYPE returns a ob_type struct
 * - ob_type.tp_name is the type name string intended for printing; as with
 * type().__name__
 *
 * @p: PyObject*-castable struct pointer
 */
void print_python_list(PyObject *p)
{
	int i;

	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %lu\n", Py_SIZE(p));
	printf("[*] Allocated = %lu\n", ((PyListObject *)p)->allocated);
	for (i = 0; i < Py_SIZE(p); i++)
		printf("Element %d: %s\n", i,
		       Py_TYPE(PyList_GetItem(p, i))->tp_name);
}
