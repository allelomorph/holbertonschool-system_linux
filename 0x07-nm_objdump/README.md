# (369) 0x07. C - ELF: nm/objdump
Specializations > System programming & Algorithm > Linux Programming

### Project author
Alexandre Gautier

### Assignment dates
03-15-2021 to 03-25-2021

### Description
Continued exploration of ELF structures, now in building clones of `nm` and `objdump`.

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
* Test ELF files:
  * [jpeg.mod](./ELF_examples/jpeg.mod)
  * [libperl.so.5.18](./ELF_examples/libperl.so.5.18)
  * [netbsd32](./ELF_examples/netbsd32)
  * [python.obj](./ELF_examples/python.obj)
  * [sftp-server](./ELF_examples/sftp-server)
  * [solaris32](./ELF_examples/solaris32)
  * [sortix32](./ELF_examples/sortix32)
  * [sparcbigendian32](./ELF_examples/sparcbigendian32)
  * [ubuntu64](./ELF_examples/ubuntu64)

---

## Mandatory Tasks

### :white_check_mark: 0. nm
Write a program that reproduces the GNU `nm` command

* Usage: `hnm [objfile ...]`
* The output should be the exact same as `nm -p`
* Your sources must be compiled and linked using a Makefile

Compiled: `make hnm`

### :white_check_mark: 1. objdump -sf
Write a program that reproduces the GNU `objdump` command

* Usage: `hobjdump [objfile ...]`
* The output should be the exact same as `objdump -sf`
* Your sources must be compiled and linked using a Makefile

Compiled: `make hobjdump`

### :white_check_mark: 2. Blog post
Write a blog post about the ELF format

Your blog post should cover the following:

* What is `ELF`
* Why it is used
* What information is stored in it
* How this information is stored
* How to parse this information
* Describe the `readelf` command
* Describe the `nm` command
* Describe the `objdump` command

Your posts should have examples and at least one picture, at the top. Publish your blog post on Medium or LinkedIn, and share it at least on Twitter and LinkedIn.

<!--
https://www.linkedin.com/pulse/introduction-elf-file-format-samuel-pomeroy
https://www.linkedin.com/posts/activity-6780627819820142592-UGnp
-->

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
