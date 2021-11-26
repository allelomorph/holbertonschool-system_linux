# (374) 0x0B. C - malloc
Specializations > System programming & Algorithm > Linux Programming

### Project author
Alexandre Gautier

### Assignment dates
05-28-2021 to 06-11-2021

### Description
Investigating virtual memory and the heap by building clones of the glibc `malloc`, `free`, `calloc`, and `realloc`.

### Requirements
* Global and static variables are allowed for these tasks.
* Test by naming your functions `malloc`, `free`, `realloc` and `calloc` (as in glibc), and compile them into a shared library that you would load when executing a program using `LD_LIBRARY_PATH` and `LD_PRELOAD` (see [here](https://www.cprogramming.com/tutorial/shared-libraries-linux-gcc.html)

### Note
This project can be found in a separate repository: [`malloc`](https://github.com/allelomorph/malloc). This README is to provide the project profile separate from the README originally included with the project itself.

### Provided file(s)
* Test mains from naive malloc concept page:
[`0-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Alex-concept-page/0-main.c) [`1-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Alex-concept-page/1-main.c) [`2-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Alex-concept-page/2-main.c) [`2-main_malloc.c`](https://github.com/allelomorph/malloc/blob/master/tests/Alex-concept-page/2-main_malloc.c) [`3-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Alex-concept-page/3-main.c) [`4-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Alex-concept-page/4-main.c) [`5-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Alex-concept-page/5-main.c) [`6-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Alex-concept-page/6-main.c) 
* Test mains from ["Hack the Virtual Memory: drawing the VM diagram"](https://blog.holbertonschool.com/hack-the-virtual-memory-drawing-the-vm-diagram/) by Julien Barbier:
[`main-2.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c2/main-2.c) [`main-4.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c2/main-4.c) [`main-6.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c2/main-6.c) [`main-7.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c2/main-7.c) 
* Test mains from ["Hack the Virtual Memory: malloc, the heap & the program break"](https://blog.holbertonschool.com/hack-the-virtual-memory-malloc-the-heap-the-program-break/) by Julien Barbier:
[`0-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c3/0-main.c) [`1-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c3/1-main.c) [`3-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c3/3-main.c) [`4-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c3/4-main.c) [`5-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c3/5-main.c) [`6-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c3/6-main.c) [`7-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c3/7-main.c) [`8-main.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c3/8-main.c) [`naive_malloc.c`](https://github.com/allelomorph/malloc/blob/master/tests/Julien-blog-c3/naive_malloc.c) 

---

## Mandatory Tasks

### :white_check_mark: 0. Naive malloc
Build the naive malloc that is presented in the Holberton **[EYNTK] Writing your own dynamic allocation library** concept page.

* Prototype: `void *naive_malloc(size_t size);`
* Where `size` is the size needed to be allocated for the user
* Your function must return a pointer to the allocated memory that is suitably aligned for any kind of variable
* You naive malloc should be able to:
    * Allocate enough memory to store
        * A `size_t` as the chunk header
        * The size requested as parameter
    * Allocate memory pages only

File(s): [`naive_malloc.c`](https://github.com/allelomorph/malloc/blob/master/naive_malloc.c) [`malloc.h`](https://github.com/allelomorph/malloc/blob/master/malloc.h)

### :white_check_mark: 1. malloc
Write you own `malloc` function that allocates space in the heap

* Prototype: `void *_malloc(size_t size);`
* Where `size` is the size needed to be allocated for the user
* Your function must return a pointer to the allocated memory that is suitably aligned for any kind of variable

File(s): [`malloc.c`](https://github.com/allelomorph/malloc/blob/master/malloc.c) [`malloc.h`](https://github.com/allelomorph/malloc/blob/master/malloc.h)\
Compiled: (into "shim" shared library) `gcc -Wall -Werror -Wextra -pedantic -shared -fPIC -ldl tests/_malloc_shim.c malloc.h malloc.c free.c  calloc.c realloc.c -o _malloc_shim.so`\
Test in place of glibc function with: `LD_PRELOAD=_malloc_shim.so <executable to shim> <arg(s)>`

### :white_check_mark: 2. free
Write you own `free` function that frees a memory space

* Prototype: `void _free(void *ptr);`
* Where `ptr` is a pointer to the memory space to be freed

File(s): [`free.c`](https://github.com/allelomorph/malloc/blob/master/free.c) [`malloc.c`](https://github.com/allelomorph/malloc/blob/master/malloc.c) [`malloc.h`](https://github.com/allelomorph/malloc/blob/master/malloc.h)\
Compiled: (into "shim" shared library) `gcc -Wall -Werror -Wextra -pedantic -shared -fPIC -ldl tests/_malloc_shim.c malloc.h malloc.c free.c  calloc.c realloc.c -o _malloc_shim.so`\
Test in place of glibc function with: `LD_PRELOAD=_malloc_shim.so <executable to shim> <arg(s)>`

## Advanced Tasks

### :white_check_mark: 3. calloc
Write you own `malloc` function that allocates space in the heap

* Prototype: `void *_calloc(size_t nmemb, size_t size);`
* Where `nmemb` is the number of elements in the array, and size is the size of each element
* Your function must return a pointer to the allocated memory that is suitably aligned for any kind of variable

File(s): [`calloc.c`](https://github.com/allelomorph/malloc/blob/master/calloc.c) [`free.c`](https://github.com/allelomorph/malloc/blob/master/free.c) [`malloc.c`](https://github.com/allelomorph/malloc/blob/master/malloc.c) [`malloc.h`](https://github.com/allelomorph/malloc/blob/master/malloc.h)\
Compiled: (into "shim" shared library) `gcc -Wall -Werror -Wextra -pedantic -shared -fPIC -ldl tests/_malloc_shim.c malloc.h malloc.c free.c calloc.c realloc.c -o _malloc_shim.so`\
Test in place of glibc function with: `LD_PRELOAD=_malloc_shim.so <executable to shim> <arg(s)>`

### :white_check_mark: 4. realloc
Write you own `malloc` function that allocates space in the heap

* Prototype: `void *_realloc(void *ptr, size_t size);`
* Where `ptr` is a pointer to the memory space to resize, and `size` is the new size needed to be allocated for the user
* Your function must return a pointer to the new allocated memory that is suitably aligned for any kind of variable

File(s): [`realloc.c`](https://github.com/allelomorph/malloc/blob/master/realloc.c) [`free.c`](https://github.com/allelomorph/malloc/blob/master/free.c) [`malloc.c`](https://github.com/allelomorph/malloc/blob/master/malloc.c) [`malloc.h`](https://github.com/allelomorph/malloc/blob/master/malloc.h)\
Compiled: (into "shim" shared library) `gcc -Wall -Werror -Wextra -pedantic -shared -fPIC -ldl tests/_malloc_shim.c malloc.h malloc.c free.c  calloc.c realloc.c -o _malloc_shim.so`\
Test in place of glibc function with: `LD_PRELOAD=_malloc_shim.so <executable to shim> <arg(s)>`

### :white_check_mark: 5. Multithreading
Handle multithreading!

Your functions `_malloc`, `_free`, `_realloc`, and `_calloc` must be thread safe

Your functions will be compiled and linked with the flag `-pthread`

Resources:

* [pthread.h](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html)
* [Thread safety](https://en.wikipedia.org/wiki/Thread_safety)

File(s): [`malloc.c`](https://github.com/allelomorph/malloc/blob/pthread/malloc.c) [`free.c`](https://github.com/allelomorph/malloc/blob/pthread/free.c) [`realloc.c`](https://github.com/allelomorph/malloc/blob/pthread/realloc.c) [`calloc.c`](https://github.com/allelomorph/malloc/blob/pthread/calloc.c) [`malloc.h`](https://github.com/allelomorph/malloc/blob/pthread/malloc.h)\
Compiled: (into "shim" shared library) `gcc -Wall -Werror -Wextra -pedantic -shared -fPIC -ldl -pthread tests/_malloc_shim.c malloc.h malloc.c free.c calloc.c realloc.c -o _malloc_shim.so`\
Test in place of glibc function with: `LD_PRELOAD=_malloc_shim.so <executable to shim> <arg(s)>`

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
