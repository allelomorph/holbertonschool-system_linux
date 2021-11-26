# (277) 0x08. CPython
Specializations > System programming & Algorithm > Linux Programming

### Project author
Julien Barbier

### Assignment dates
03-25-2021 to 04-02-2021

### Description
An investigation into how Python 3 is implemented in C and how to access it via its C API.

### Requirements
Big O Notation in Answer File
* `O(1)`
* `O(n)`
* `O(n!)`
* n square -> `O(n^2)`
* log(n) -> `O(log(n))`
* n * log(n) -> `O(nlog(n))`
* “short” notation (don’t use constants)

### Provided file(s)
[`1-test_lists.py`](./tests/1-test_lists.py) [`2-tests.py`](./tests/2-tests.py) [`3-tests.py`](./tests/3-tests.py) [`4-tests.py`](./tests/4-tests.py) [`5-tests.py`](./tests/5-tests.py) [`100-tests.py`](./tests/100-tests.py)

---

## Mandatory Tasks

### :white_check_mark: 0. Lists big O
Write the big O notations for the following lists operations (one per line, in this exact order):
* L1 - Assigning. Example: `holberton[98] = 402`
* L2 - Appending. Example: `holberton.append(98)`
* L3 - Concatenating a list of n elements. Example: `holberton + [98, 402]`
* L4 - Inserting an item at a given position. Example: `holberton.insert(98, 402)`
* L5 - Removing an element. Example: `holberton.remove(98)`
* L6 - Removing and returning the last item in the list. Example: `holberton.pop()`
* L7 - Poping an element at a given position. Example: `holberton.pop(98)`
* L8 - Clearing a list. Example: `holberton.clear()`
* L9 - Counting the list. Example: `holberton.count()`
* L10 - Sorting the list. Example: `holberton.sort()`
* L11 - Reversing the list. Example: `holberton.reverse()`
* L12 - Deleting the list. Example: `del holberton`

File(s): [`0-python_lists_bigO`](./0-python_lists_bigO)

### :white_check_mark: 1. CPython #0: Python lists
CPython is the reference implementation of the Python programming language. Written in C, CPython is the default and most widely used implementation of the language.

Create a C function that prints some basic info about Python lists.

* Prototype: void print_python_list(PyObject *p);
* Python version: 3.4
* Your shared library will be compiled with this command line: gcc -Wall -Werror -Wextra -pedantic -std=c99 -shared -Wl,-soname,PyList -o libPyList.so -fPIC -I/usr/include/python3.4 1-python.c
* OS: Ubuntu 14.04 LTS
* Start by reading:
    * `listobject.h`
    * `object.h`
    * [Common Object Structures](https://docs.python.org/3.4/c-api/structures.html)
    * [List Objects](https://docs.python.org/3.4/c-api/list.html)

File(s): [`1-python.c`](./1-python.c)\
Compiled: `gcc -Wall -Werror -Wextra -pedantic -std=c99 -shared -Wl,-soname,PyList -o libPyList.so -fPIC -I/usr/include/python3.4 1-python.c`

### :white_check_mark: 2. CPython #1: PyBytesObject
Create two C functions that print some basic info about Python lists and Python bytes objects.

Python lists:
* Prototype: `void print_python_list(PyObject *p);`

Python bytes:
* Prototype: `void print_python_bytes(PyObject *p);`
* Line “first X bytes”: print a maximum of 10 bytes
* If `p` is not a valid `PyBytesObject`, print an error message (see example)
* Read `/usr/include/python3.4/bytesobject.h`

About:
* Python version: 3.4
* Your shared library will be compiled with this command line: `gcc -Wall -Werror -Wextra -pedantic -std=c99 -shared -Wl,-soname,libPython.so -o libPython.so -fPIC -I/usr/include/python3.4 2-python.c`
* You are not allowed to use the following macros/functions:
    * `Py_SIZE`
    * `Py_TYPE`
    * `PyList_GetItem`
    * `PyBytes_AS_STRING`
    * `PyBytes_GET_SIZE`

File(s): [`2-python.c`](./2-python.c)\
Compiled: `gcc -Wall -Werror -Wextra -pedantic -std=c99 -shared -Wl,-soname,libPython.so -o libPython.so -fPIC -I/usr/include/python3.4 2-python.c`

### :white_check_mark: 3. CPython #2: PyFloatObject
Create three C functions that print some basic info about Python lists, Python bytes an Python float objects.

Python lists:
* Prototype: `void print_python_list(PyObject *p);`
* If `p` is not a valid `PyListObject`, print an error message

Python bytes:
* Prototype: `void print_python_bytes(PyObject *p);`
* Line “first X bytes”: print a maximum of 10 bytes
* If `p` is not a valid `PyBytesObject`, print an error message

Python float:
* Prototype: `void print_python_float(PyObject *p);`
* If `p` is not a valid `PyFloatObject`, print an error message
* Read `/usr/include/python3.4/floatobject.h`

About:
* Python version: 3.4
* You are allowed to use the C standard library
* Your shared library will be compiled with this command line: `gcc -Wall -Werror -Wextra -pedantic -std=c99 -shared -Wl,-soname,libPython.so -o libPython.so -fPIC -I/usr/include/python3.4 3-python.c`
* You are not allowed to use the following macros/functions:
    * `Py_SIZE`
    * `Py_TYPE`
    * `PyList_Size`
    * `PyList_GetItem`
    * `PyBytes_AS_STRING`
    * `PyBytes_GET_SIZE`
    * `PyBytes_AsString`
    * `PyFloat_AS_DOUBLE`
    * `PySequence_GetItem`
    * `PySequence_Fast_GET_SIZE`
    * `PySequence_Fast_GET_ITEM`
    * `PySequence_ITEM`
    * `PySequence_Fast_ITEMS`

**NOTE:**
* The Python script will be launched using the `-u` option (force stdout to be unbuffered).
* It is **strongly** advised to either use `setbuf(stdout, NULL);` or `fflush(stdout)` in your C functions IF you choose to use printf. The reason to that is that Python's `print` and libC's `printf` don’t share the same buffer, and the output can appear disordered.

File(s): [`3-python.c`](./3-python.c)\
Compiled: `gcc -Wall -Werror -Wextra -pedantic -std=c99 -shared -Wl,-soname,libPython.so -o libPython.so -fPIC -I/usr/include/python3.4 3-python.c`

### :white_check_mark: 4. CPython #3: Python Strings
Create a function that prints Python strings.
* Prototype: `void print_python_string(PyObject *p);`
* If `p` is not a valid string, print an error message
* Read: [Unicode HOWTO](https://docs.python.org/3.4/howto/unicode.html)

About:
* Python version: 3.4
* You are allowed to use the C standard library
* Your shared library will be compiled with this command line: `gcc -Wall -Wextra -pedantic -Werror -std=c99 -shared -Wl,-soname,libPython.so -o libPython.so -fPIC -I/usr/include/python3.4 4-python.c`

File(s): [`4-python.c`](./4-python.c)\
Compiled: `gcc -Wall -Wextra -pedantic -Werror -std=c99 -shared -Wl,-soname,libPython.so -o libPython.so -fPIC -I/usr/include/python3.4 4-python.c`

### :white_check_mark: 5. CPython Integers
Create a C function that prints Python integers.
* Prototype: `void print_python_int(PyObject *p);`
* If `p` is not a valid string, print an error message
* The function should first translate the Python int to a `C unsigned long int` and then use `printf` to print the number
    * If the absolute value of the number is too big to be stored in a C `unsigned long int`, print `C unsigned long int overflow`, followed by a new line
* Read: `/usr/include/python3.4/longintrepr.h`

About:
* Python version: 3.4
* You are allowed to use the C standard library
* You are only allowed to use the following Python macros / functions:
    * `PyLong_SHIFT`
    * `PyLong_Check`
* Your shared library will be compiled with this command line: `gcc -Wall -Wextra -pedantic -Werror -std=c99 -shared -Wl,-soname,libPython.so -o libPython.so -fPIC -I/usr/include/python3.4 5-python.c`

File(s): [`5-python.c`](./5-python.c)\
Compiled: `gcc -Wall -Wextra -pedantic -Werror -std=c99 -shared -Wl,-soname,libPython.so -o libPython.so -fPIC -I/usr/include/python3.4 5-python.c`

## Advanced Tasks

### :white_check_mark: 6. CPython Integers
Create a C function that prints Python integers.
* Prototype: `void print_python_int(PyObject *p);`
* If `p` is not a valid string, print an error message
* The function should be able to print any integer
* Read: `/usr/include/python3.4/longintrepr.h`

About:
* Python version: 3.4
* You are allowed to use the C standard library
* You are only allowed to use the following Python macros / functions:
    * `PyLong_SHIFT`
    * `PyLong_Check`
* Your shared library will be compiled with this command line: `gcc -Wall -Wextra -pedantic -Werror -std=c99 -shared -Wl,-soname,libPython.so.2 -o libPython.so.2 -fPIC -I/usr/include/python3.4 100-python.c`

File(s): [`100-python.c`](./100-python.c)\
Compiled: `gcc -Wall -Wextra -pedantic -Werror -std=c99 -shared -Wl,-soname,libPython.so.2 -o libPython.so.2 -fPIC -I/usr/include/python3.4 100-python.c`

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
