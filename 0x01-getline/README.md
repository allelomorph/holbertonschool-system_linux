# (322) 0x01. C - Static variables, getline
Specializations > System programming & Algorithm > Linux Programming

### Project author
Julien Barbier

### Assignment dates
12-07-2020 to 12-15-2020

### Description
Introduction to static variables in C and building a clone of the glibc function `getline`.

### Requirements
* Allowed Functions and System Calls
  * `read`
  * `write`
  * `malloc`
  * `realloc`
  * `free`
  * `strcpy`
  * `strncpy`
  * `strcat`
  * `strdup`
  * `memset`
  * `memcpy`

### Provided file(s)
* [`man_cat`](./tests/man_cat) [`man_gcc`](./tests/man_gcc) [`the_swing`](./tests/the_swing)
* [`zero`](./tests/zero)
* [`0-main.c`](./tests/0-main.c) [`1-main.c`](./tests/1-main.c) [`2-main.c`](./tests/2-main.c) [`3-main.c`](./tests/3-main.c)

---

## Mandatory Tasks

### :white_check_mark: 0. Racing cars
Write a function that keeps track of the number of laps made by several cars in a race.
  * Prototype: `void race_state(int *id, size_t size)`
  * `id` is an array of `int` representing the “identifier” of each car.
  * `size` is the size of this array
  * Each car identifier is unique
  * If an identifier is unknown:
    * Create a new car with the number of laps = 0
    * Print `Car X joined the race` followed by a new line (where `X` is the identifier) 
  * Each time the function is called:
    * The number of laps of each cars listed in `id` must be incremented by 1
    * Print the state of the race:
      * Header: `Race state:` followed by a new line
      * For each car sorted by the identifier: `Car X [Y laps]` (where `X` is the identifier and `Y` the number of laps already done) 
  * If your function is called with `size = 0`, you must free all allocated memory.

File(s): [`laps.c`](./laps.c) [`laps.h`](./laps.h)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 0-main.c laps.c  -o laps`

### :white_check_mark: 1. _getline
Write a function that reads an entire line from a file descriptor.
  * Prototype: `char *_getline(const int fd)`
  * Where `fd` is the file descriptor to read from
  * If there are no more lines to return, or if there is an error, the function should return NULL 
  * The function returns a null-terminated string that does not include the newline character 
  * Your header file `_getline.h` should define a macro called `READ_SIZE`.
    * This macro defines the number of bytes you will read each time you will call `read`: `read(fd, buffer, READ_SIZE)`
    * You are not allowed to read more or less than `READ_SIZE` bytes at once from `fd`
    * You are free to pick the value that you want for `READ_SIZE`
  * You can assume that `fd` will always be the same

File(s): [`_getline.c`](./_getline.c) [`_getline.h`](./_getline.h)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 1-main.c _getline.c -o getline`

### :white_check_mark: 2. _getline: multi-fd
Handle multiple file descriptors.
* When called with `-1` you should free everything and reset all your static variables

File(s): [`_getline.c`](./_getline.c) [`_getline.h`](./_getline.h)\
Compiled: `gcc -Wall -Wextra -pedantic -Werror 2-main.c _getline.c -o getline`

## Advanced Tasks

### :white_check_mark: 3. _getline: ^@
Handle characters `\0` in lines.
* Here’s the file used for the example below: [zero](./tests/zero)

```bash
$ cat zero
line 1 Holberton.....
line 2 still line 2
line 3 ......School()
$ gcc -g -Wall -Wextra -pedantic 100-main.c _getline.c && ./a.out
6c 69 6e 65 20 31 20 48 6f 6c 62 65 72 74 6f 6e 2e 2e 2e 2e 2e
6c 69 6e 65 20 32 20 00 73 74 69 6c 6c 20 6c 69 6e 65 20 32 00
6c 69 6e 65 20 33 20 2e 2e 2e 2e 2e 2e 53 63 68 6f 6f 6c 28 29
$
```

File(s): [`_getline.c`](./_getline.c) [`_getline.h`](./_getline.h)\
Compiled: `gcc -Wall -Wextra -pedantic -Werror 100-main.c _getline.c -o getline`

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
