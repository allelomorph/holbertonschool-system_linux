# (308) 0x00. C - ls
Specializations > System programming & Algorithm > Linux Programming

### Project author
Wilfried Hennuyer

### Assignment dates
11-30-2020 to 12-04-2020

### Description
Building a clone of the Linux function `ls`.

### Requirements
* Allowed Functions and System Calls
  * opendir (man 3 opendir)
  * readdir (man 3 readdir)
  * closedir (man 3 closedir)
  * exit (man 3 exit)
  * free (man 3 free)
  * lstat (man 2 lstat)
  * malloc (man 3 malloc)
  * perror (man 3 perror)
  * write (man 2 write)
  * printf (man 3 printf)
  * sprintf (man 3 sprintf)
  * fprintf (man 3 fprintf)
  * readlink (man 2 readlink)
  * ctime (man 3 ctime)
  * getpwuid (man 3 getpwuid)
  * getgrgid (man 3 getgrgid)
  * errno (man 3 errno)
* Compiled: `gcc -Wall -Werror -Wextra -pedantic *.c -o hls`
* Your program should give the same result as the real `ls`:
```bash
$ ls test
abc  BCD  file  file2  file3  folder1  folder2  folder3
$ ./hls test
abc  BCD  file  file2  file3  folder1  folder2  folder3
$ ls -1
abc
BCD
file
file2
file3
folder1
folder2
folder3
hls
$ ./hls -1
abc
BCD
file
file2
file3
folder1
folder2
folder3
hls
$
```
* For all tasks of this project, unless explicitly said otherwise:
  * You don’t have to worry about the spacing
  * You don’t have to worry about the sorting

### Provided file(s)

---

## Mandatory Tasks

### :white_large_square: 0. Let's start with something simple!
Create a program that lists the content of the current directory.
* Usage: `hls`

### :white_large_square: 1. Maybe some parameters?
Your program should now take one or more file or directory names as parameters.
* Usage: `hls [FILE]...`
* Errors must be printed the same way than `ls` does:
  * In `stderr`
  * Starting by `<program>`: (`<program>` being `argv[0]`)
  * `No such file or directory`, `Permission denied`, etc…
  * Don’t forget the exit value

### :white_large_square: 2. What about options?
Implement the `-1` option.
* Usage: `hls [-1] [FILE]...`
* For the rest of the project, an option will be identified by a command-line argument starting with the character `-` (like `ls`).
* Pay attention to “edge cases”: you should be able to handle multiple options, in any order.

### :white_check_mark: 3. Hidden files
Implement the `-a` option.
* Usage: `hls [-a1] [FILE]...`

### :white_check_mark: 4. Almost all
Implement the `-A` option.
* Usage: `hls [-A] [FILE]...`

### :white_check_mark: 5. More details
Implement the `-l` option.
* Usage: `hls [-l] [FILE]...`
* You might notice the `total XX` in the first line of `ls -l`. You can ignore this value for this exercise.
* Pay attention to “edge cases”: Usernames and group names are not always defined.

## Advanced Tasks

### :white_check_mark: 6. Mixing options
All options together.
* Usage: `hls [-1aAl] [FILE]...`

### :white_check_mark: 7. Esrever
Implement the `-r` option.
* Usage: `hls [-1aAlr] [FILE]...`

### :white_check_mark: 8. More sorting?
Implement the `-S` option.
Usage: `hls [-1aAlrS] [FILE]...`

### :white_check_mark: 9. I know you like sorting things.
Implement the `-t` option.
* Usage: `hls [-1aAlrSt] [FILE]...`

### :white_check_mark: 10. The Juggernaut
Implement the `-R` option.
* Usage: `hls [-1aAlrStR] [FILE]...`

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
