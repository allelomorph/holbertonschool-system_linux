# (376) 0x05. x86 Assembly - libASM
Specializations > System programming & Algorithm > Linux Programming

### Project author
Alexandre Gautier

### Assignment dates
02-01-2021 to 02-12-2021

### Description
Introduction to x86 assembly: Intel vs AT&T, flag registers, stack frames, function and system calls.

### Requirements
* Allowed Functions and System Calls
Unless specified otherwise, you are NOT allowed to do any call or make any system call. It means you’re not allowed to use either the `call` nor the `syscall` instructions.
* ASM programs and functions expected to be compiled with NASM version 2.10.09 using the flags `-f elf64`

### Provided file(s)
* [`0-main.c`](./tests/0-main.c) [`1-main.c`](./tests/1-main.c) [`2-main.c`](./tests/2-main.c) [`3-main.c`](./tests/3-main.c) [`4-main.c`](./tests/4-main.c) [`5-main.c`](./tests/5-main.c) [`6-main.c`](./tests/6-main.c) [`7-main.c`](./tests/7-main.c) [`8-main.c`](./tests/8-main.c) [`9-main.c`](./tests/9-main.c) [`10-main.c`](./tests/10-main.c) [`11-main.c`](./tests/11-main.c) [`12-main.c`](./tests/12-main.c)
* [`100-main.c`](./tests/100-main.c) [`101-main.c`](./tests/101-main.c)

---

## Mandatory Tasks

### :white_check_mark: 0. strlen
Write a copycat of the function `strlen`(3), in x86-64 Assembly

* Prototype when used in C: `size_t asm_strlen(const char *str);`

File(s): [`0-strlen.asm`](./0-strlen.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 0-main.o 0-main.c
$ nasm -f elf64 -o 0-strlen.o 0-strlen.asm
$ gcc -o 0-strlen 0-main.o 0-strlen.o
```

### :white_check_mark: 1. strcmp
Write a copycat of the function `strcmp`(3), in x86-64 Assembly

* Prototype when used in C: `int asm_strcmp(const char *s1, const char *s2);`

File(s): [`1-strcmp.asm`](./1-strcmp.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 1-main.o 1-main.c
$ nasm -f elf64 -o 1-strcmp.o 1-strcmp.asm
$ gcc -o 1-strcmp 1-main.o 1-strcmp.o
```

### :white_check_mark: 2. strncmp
Write a copycat of the function `strncmp`(3), in x86-64 Assembly

* Prototype when used in C: `int asm_strncmp(const char *s1, const char *s2, size_t n);`

File(s): [`2-strncmp.asm`](./2-strncmp.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 2-main.o 2-main.c
$ nasm -f elf64 -o 2-strncmp.o 2-strncmp.asm
$ gcc -o 2-strncmp 2-main.o 2-strncmp.o
```

### :white_check_mark: 3. strchr
Write a copycat of the function `strchr`(3), in x86-64 Assembly

* Prototype when used in C: `char *asm_strchr(const char *s, int c);`

File(s): [`3-strchr.asm`](./3-strchr.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 3-main.o 3-main.c
$ nasm -f elf64 -o 3-strchr.o 3-strchr.asm
$ gcc -o 3-strchr 3-main.o 3-strchr.o
```

### :white_check_mark: 4. strstr
Write a copycat of the function `strstr`(3), in x86-64 Assembly

* Prototype when used in C: `char *asm_strstr(const char *haystack, const char *needle);`

File(s): [`4-strstr.asm`](./4-strstr.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 4-main.o 4-main.c
$ nasm -f elf64 -o 4-strstr.o 4-strstr.asm
$ gcc -o 4-strstr 4-main.o 4-strstr.o
```

### :white_check_mark: 5. memcpy
Write a copycat of the function `memcpy`(3), in x86-64 Assembly

* Prototype when used in C: `void *asm_memcpy(void *dest, const void *src, size_t n);`

File(s): [`5-memcpy.asm`](./5-memcpy.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 5-main.o 5-main.c
$ nasm -f elf64 -o 5-memcpy.o 5-memcpy.asm
$ gcc -o 5-memcpy 5-main.o 5-memcpy.o
```

### :white_check_mark: 6. putc
Write a function that prints a single character on the standard output, in x86-64 Assembly

* Prototype when used in C: `size_t asm_putc(int c);`
* Where `c` holds the character to be printed
* Your function must return the total number of bytes written on the standard output
* For this task, you are allowed to use the `syscall` instruction only once in your file

File(s): [`6-putc.asm`](./6-putc.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 6-main.o 6-main.c
$ nasm -f elf64 -o 6-putc.o 6-putc.asm
$ gcc -o 6-putc 6-main.o 6-putc.o
```

### :white_check_mark: 7. puts
Write a function that prints a string of characters on the standard output, in x86-64 Assembly

* Prototype when used in C: size_t `asm_puts(const char *str);`
* Where `str` holds the string to be printed
* Your function must return the total number of bytes written on the standard output
* You are not allowed to use any sort of `jump`
* Your file `0-strlen.asm` will be compiled as well, you are allowed to `call` it once in your file
* For this task, you are allowed to use the `syscall` instruction only once in your file

File(s): [`7-puts.asm`](./7-puts.asm) [`0-strlen.asm`](./0-strlen.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 7-main.o 7-main.c
$ nasm -f elf64 -o 7-puts.o 7-puts.asm
$ nasm -f elf64 -o 0-strlen.o 0-strlen.asm
$ gcc -o 7-puts 7-main.o 7-puts.o 0-strlen.o
```

### :white_check_mark: 8. strcasecmp
Write a copycat of the function `strcasecmp`(3), in x86-64 Assembly

* Prototype when used in C: `int asm_strcasecmp(const char *s1, const char *s2);`

File(s): [`8-strcasecmp.asm`](./8-strcasecmp.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 8-main.o 8-main.c
$ nasm -f elf64 -o 8-strcasecmp.o 8-strcasecmp.asm
$ gcc -o 8-strcasecmp 8-main.o 8-strcasecmp.o
```

### :white_check_mark: 9. strncasecmp
Write a copycat of the function `strncasecmp`(3), in x86-64 Assembly

* Prototype when used in C: `int asm_strncasecmp(const char *s1, const char *s2, size_t n);`

File(s): [`9-strncasecmp.asm`](./9-strncasecmp.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 9-main.o 9-main.c
$ nasm -f elf64 -o 9-strncasecmp.o 9-strncasecmp.asm
$ gcc -o 9-strncasecmp 9-main.o 9-strncasecmp.o
```

### :white_check_mark: 10. strspn
Write a copycat of the function `strspn`(3), in x86-64 Assembly

* Prototype when used in C: `size_t asm_strspn(const char *s, const char *accept);`

File(s): [`10-strspn.asm`](./10-strspn.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 10-main.o 10-main.c
$ nasm -f elf64 -o 10-strspn.o 10-strspn.asm
$ gcc -o 10-strspn 10-main.o 10-strspn.o
```

### :white_check_mark: 11. strcspn
Write a copycat of the function `strcspn`(3), in x86-64 Assembly

* Prototype when used in C: `size_t asm_strcspn(const char *s, const char *reject);`

File(s): [`11-strcspn.asm`](./11-strcspn.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 11-main.o 11-main.c
$ nasm -f elf64 -o 11-strcspn.o 11-strcspn.asm
$ gcc -o 11-strcspn 11-main.o 11-strcspn.o
```

### :white_check_mark: 12. strpbrk
Write a copycat of the function `strpbrk`(3), in x86-64 Assembly

* Prototype when used in C: `char *asm_strpbrk(const char *s, const char *accept);`

File(s): [`12-strpbrk.asm`](./12-strpbrk.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 12-main.o 12-main.c
$ nasm -f elf64 -o 12-strpbrk.o 12-strpbrk.asm
$ gcc -o 12-strpbrk 12-main.o 12-strpbrk.o
```

## Advanced Tasks

### :white_check_mark: 13. puti
Write a function that prints a signed integer on the standard output, in x86-64 Assembly

* Prototype when used in C: `size_t asm_puti(int n);`
* Where `n` holds the integer to be printed
* Your function must return the total number of bytes written on the standard output
* Your file `6-putc.asm` will be compiled as well
* You are allowed to use the instruction `call` up to three times in your file

File(s): [`100-puti.asm`](./100-puti.asm) [`6-putc.asm`](./6-putc.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 100-main.o 100-main.c
$ nasm -f elf64 -o 100-puti.o 100-puti.asm
$ nasm -f elf64 -o 6-putc.o 6-putc.asm
$ gcc -o 100-puti 100-main.o 100-puti.o 6-putc.o
```

### :white_check_mark: 14. puti_base
Write a function that prints a signed integer in a given base on the standard output, in x86-64 Assembly

* Prototype when used in C: `size_t asm_puti_base(int n, const char *base);`
* Where `n` holds the integer to be printed
* And `base` points to a string representing the base used to print `n`
* Your function must return the total number of bytes written on the standard output
* Your files `6-putc.asm` and `0-strlen.asm` will be compiled as well
* You are allowed to use the instruction `call` up to four times in your file

File(s): [`101-puti_base.asm`](./101-puti_base.asm) [`6-putc.asm`](./6-putc.asm) [`0-strlen.asm`](./0-strlen.asm)\
Compiled:
```bash
$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 101-main.o 101-main.c
$ nasm -f elf64 -o 101-puti_base.o 101-puti_base.asm
$ nasm -f elf64 -o 6-putc.o 6-putc.asm
$ nasm -f elf64 -o 0-strlen.o 0-strlen.asm
$ gcc -o 101-puti_base 101-main.o 101-puti_base.o 6-putc.o 0-strlen.o
```

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
