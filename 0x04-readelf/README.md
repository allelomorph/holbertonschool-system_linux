# (364) 0x04. C - ELF: readelf
Specializations > System programming & Algorithm > Linux Programming

### Project author
Alexandre Gautier

### Assignment dates
01-21-2021 to 01-29-2021

### Description
Introduction to ELF or the Executable and Linkable Format, its uses, and structure.

### Requirements
Allowed Functions and System Calls

* Unless specified otherwise, you are allowed to use the C standard library
* You’re not allowed to use `system`(3)
* You’re not allowed to use `exec`(2) and `exec`(3)

Tests

* Your program must be able to handle both 32-bit and 64-bit ELF files
* Your program must be able to handle both little and big endian ELF files
* Your program must be able to handle all types of ELF files


### Provided file(s)

---

## Mandatory Tasks

### :white_check_mark: 0. ELF file header
Write a program that displays the information contained in the ELF file header of an ELF file.

* Usage: `0-hreadelf elf_filename`
* Your standard output, error output and status should be the exact same as `readelf -W -h`

Your makefile must define the rule `0-hreadelf` and compile the needed sources to form the executable `0-hreadelf`

Compiled: `make 0-hreadelf`

### :white_check_mark: 1. ELF sections' headers
Write a program that displays the information contained in the ELF sections' headers of an ELF file.

* Usage: `1-hreadelf elf_filename`
* Your standard output, error output and status should be the exact same as `readelf -W -S`

Your makefile must define the rule `1-hreadelf` and compile the needed sources to form the executable `1-hreadelf`

Compiled: `make 1-hreadelf`

### :white_check_mark: 2. ELF program headers
Write a program that displays the information contained in the ELF program headers of an ELF file.

* Usage: `2-hreadelf elf_filename`
* Your standard output, error output and status should be the exact same as `readelf -W -l`

Your makefile must define the rule `2-hreadelf` and compile the needed sources to form the executable `2-hreadelf`

Compiled: `make 2-hreadelf`

## Advanced Tasks

### :white_check_mark: 3. ELF symbol table
Write a program that displays the information contained in the ELF symbol tables of an ELF file.

* Usage: `100-hreadelf elf_filename`
* Your standard output, error output and status should be the exact same as `readelf -W -s`

Your makefile must define the rule `100-hreadelf` and compile the needed sources to form the executable `100-hreadelf`

Compiled: `make 100-hreadelf`

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
