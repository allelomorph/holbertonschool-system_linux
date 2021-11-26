# (393) 0x09. C - Strace
Specializations > System programming & Algorithm > Linux Programming

### Project author
Alexandre Gautier

### Assignment dates
04-15-2021 to 04-23-2021

### Description
Learning more about syscalls and registers, and the use of the `ptrace` library through cloning behavior of `strace`.

### Requirements

### Provided file(s)
[`syscalls.h`](./syscalls.h) (modified, original [here](https://github.com/holbertonschool/0x0B-strace.c/blob/master/syscalls.h))

---

## Mandatory Tasks

### :white_check_mark: 0. Step #0 - Syscall number
Write a program that executes and traces a given command.

* Usage: `./strace_0 command [args...]`
* Each time a syscall is intercepted, you must print its number, followed by a new line
* You don’t have to handle the `PATH: command` will be a full path to a binary (Ex: `/bin/ls` and not `ls`)
* Your Makefile must implement a rule `strace_0`

Compiled: `make strace_0`

### :white_check_mark: 1. Step #1 - Syscall name
Write a program that executes and traces a given command.

* Usage: `./strace_1 command [args...]`
* Each time a syscall is intercepted, you must print its name, followed by a new line
* You don’t have to handle the `PATH: command` will be a full path to a binary (Ex: `/bin/ls` and not `ls`)
* Your Makefile must implement a rule `strace_1`

Compiled: `make strace_1`

### :white_check_mark: 2. Step #2 - Return value
Write a program that executes and traces a given command.

* Usage: `./strace_2 command [args...]`
* Each time a syscall is intercepted, you must print its name and its return value in hexadecimal, followed by a new line
* You don’t have to handle the `PATH: command` will be a full path to a binary (Ex: `/bin/ls` and not `ls`)
* Your Makefile must implement a rule `strace_2`
* NOTES: It is impossible to retrieve the last system call return value, simply because it doesn’t return. So, just print a `?` like strace does.

Compiled: `make strace_2`

### :white_check_mark: 3. Step #3 - Raw parameters
Write a program that executes and traces a given command.

* Usage: `./strace_3 command [args...]`
* Each time a syscall is intercepted, you must print its name, its parameters in hexadecimal, and its return value in hexadecimal, followed by a new line (see example)
* There’s a difficulty: The number of parameters is not the same for all syscalls…
* You DON’T have to handle `varargs`:
    * In case of a variadic system call, just print `...`
    * Example with the `ioctl` syscall: `ioctl(0x3, 0x1, ...) = 0` (These are random values…)
* You don’t have to handle the `PATH: command` will be a full path to a binary (Ex: `/bin/ls` and not `ls`)
* Your Makefile must implement a rule `strace_3`
* NOTE: Refer to the previous `x86-64_Assembly` project to know where to find the arguments

Compiled: `make strace_3`

## Advanced Tasks

### :white_check_mark: 4. Step #4 - Strings
Write a program that executes and traces a given command.

* Usage: `./strace_4 command [args...]`
* Same as the previous task (Step #3), but you must display `char *` parameters
* Your Makefile must implement a rule `strace_4`

Quick tip: For the first syscall (`execve`), you have to print the parameters yourself, you cannot retrieve them. Why? Remember how `execve` works? It “overrides” the process memory with the binary we give it the path to. So, to simplify, `ptrace` will detect that `execve` is called, but when it is, the memory of your child process has been overwritten, so it’s impossible to get the parameters.

Compiled: `make strace_4`

### :white_check_mark: 5. Step #5 - Integers
Write a program that executes and traces a given command.

* Usage: `./strace_5 command [args...]`
* Same as the previous task (Step #4), but you must display integer parameters and return values. You must handle the following types:
    * `int`
    * `long`
    * `size_t`
    * `ssize_t`
    * `u64`
    * `uint32_t`
    * `unsigned int`
    * `unsigned long`
    * `pid_t`
* Your Makefile must implement a rule `strace_5`
* NOTE: On failure, most syscalls return -1. You might get other values when you retrieve return values. This is simply the negated `errno` code, so don’t worry about that now.

Compiled: `make strace_5`

### :white_check_mark: 6. Step #6 - Macros
Write a program that executes and traces a given command.

* Usage: `./strace_6 command [args...]`
* Same as the previous task (Step #5), plus you must interpret the following macros and flags:
    * `NULL`
    * All the flags for the `mmap`, `open` and `access` functions
* Your Makefile must implement a rule strace_6`

Compiled: `make strace_6`

### :white_check_mark: 7. Step #7 - Buffer
Write a program that executes and traces a given command.

* Usage: `./strace_7 command [args...]`
* Same as the previous task (Step #6), plus you must interpret the buffers for the read and write functions
    * If the buffer size is greater than 32, you must print only the first 32 characters, followed by `...`
    * Non printable characters should be written `\OCT` where `OCT` is the ascii code in octal, excepted for the following: `\a`, `\b`, `\t`, `\n`, `\v`, `\f` and `\r`.
* Your Makefile must implement a rule `strace_7`

Compiled: `make strace_7`

### :white_check_mark: 8. Step #8 - Structure
Write a program that executes and traces a given command.

* Usage: `./strace_8 command [args...]`
* Same as the previous task (Step #7), plus you must interpret the structures for the `fstat` function
    * You must only print `st_mode` and `st_size`, followed by `...`
* Your Makefile must implement a rule `strace_8`

Compiled: `make strace_8`

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
