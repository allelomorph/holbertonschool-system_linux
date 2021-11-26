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

| &nbsp; | &nbsp; | &nbsp; | &nbsp; | &nbsp; |
| ------ | ------ | ------ | ------ | ------ |
| `opendir`(3) | `readdir`(3) | `closedir`(3) | `exit`(3) | `free`(3) |
| `lstat`(2) | `malloc`(3) | `perror`(3) | `write`(2) | `printf`(3) |
| `sprintf`(3) | `fprintf`(3) | `readlink`(2) | `ctime`(3) | `getpwuid`(3) |
| `getgrgid`(3) | `errno`(3) | | | |

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

```bash
$ ls
abc  file  File2  file3  folder1  Folder2  folder3  hls
$ ./hls
abc  file  File2  file3  folder1  Folder2  folder3  hls
$
```

### :white_large_square: 2. What about options?
Implement the `-1` option.
* Usage: `hls [-1] [FILE]...`
* For the rest of the project, an option will be identified by a command-line argument starting with the character `-` (like `ls`).
* Pay attention to “edge cases”: you should be able to handle multiple options, in any order.

```bash
$ ls test
abc  file  File2  file3  folder1  Folder2  folder3
$ ./hls test
abc  file  File2  file3  folder1  Folder2  folder3
$ ./hls test/folder1/ test
test:
abc  file  File2  file3  folder1  Folder2  folder3

test/folder1/:
folder1-1  folder1-2  folder1-3
/simple_ls$ ./hls test/random
./hls: cannot access test/random: No such file or directory
$
```

### :white_check_mark: 3. Hidden files
Implement the `-a` option.
* Usage: `hls [-a1] [FILE]...`

```bash
$ ls test -1
abc
file
File2
file3
folder1
Folder2
folder3
$ ./hls test -1
abc
file
File2
file3
folder1
Folder2
folder3
$
```

### :white_check_mark: 4. Almost all
Implement the `-A` option.
* Usage: `hls [-A] [FILE]...`

```bash
$ ls -a test
.  ..  abc  file  File2  file3  folder1  Folder2  folder3  .hidden1  .hidden2  ..hidden3
$ ./hls -a test
.  ..  abc  file  File2  file3  folder1  Folder2  folder3  .hidden1  .hidden2  ..hidden3
$ ./hls -a test/folder1
.  ..  folder1-1  folder1-2  folder1-3
$ 
```

### :white_check_mark: 5. More details
Implement the `-l` option.
* Usage: `hls [-l] [FILE]...`
* You might notice the `total XX` in the first line of `ls -l`. You can ignore this value for this exercise.
* Pay attention to “edge cases”: Usernames and group names are not always defined.

```bash
$ ls -l test
total 12
-rw-rw-r-- 1 vagrant vagrant    0 May  8 00:19 abc
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 File2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file3
drwxrwxr-x 5 vagrant vagrant 4096 Apr 30 15:22 folder1
drwxrwxr-x 2 vagrant vagrant 4096 May  7 15:36 Folder2
drwxrwxr-x 3 vagrant vagrant 4096 May  7 15:37 folder3
$ ./hls -l test
-rw-rw-r-- 1 vagrant vagrant    0 May  8 00:19 abc
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 File2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file3
drwxrwxr-x 5 vagrant vagrant 4096 Apr 30 15:22 folder1
drwxrwxr-x 2 vagrant vagrant 4096 May  7 15:36 Folder2
drwxrwxr-x 3 vagrant vagrant 4096 May  7 15:37 folder3
$
```

## Advanced Tasks

### :white_check_mark: 6. Mixing options
All options together.
* Usage: `hls [-1aAl] [FILE]...`

```bash
$ ./hls -lA test
-rw-rw-r-- 1 vagrant vagrant    0 May  8 00:19 abc
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 File2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file3
drwxrwxr-x 5 vagrant vagrant 4096 Apr 30 15:22 folder1
drwxrwxr-x 2 vagrant vagrant 4096 May  7 15:36 Folder2
drwxrwxr-x 3 vagrant vagrant 4096 May  7 15:37 folder3
-rw-rw-r-- 1 vagrant vagrant    0 Apr 30 16:30 .hidden1
-rw-rw-r-- 1 vagrant vagrant    0 Apr 30 16:30 .hidden2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 30 17:08 ..hidden3
$ ./hls -lA -a test
drwxrwxr-x 5 vagrant vagrant 4096 May  8 01:12 .
drwxrwxr-x 4 vagrant vagrant 4096 May 15 11:51 ..
-rw-rw-r-- 1 vagrant vagrant    0 May  8 00:19 abc
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 File2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file3
drwxrwxr-x 5 vagrant vagrant 4096 Apr 30 15:22 folder1
drwxrwxr-x 2 vagrant vagrant 4096 May  7 15:36 Folder2
drwxrwxr-x 3 vagrant vagrant 4096 May  7 15:37 folder3
-rw-rw-r-- 1 vagrant vagrant    0 Apr 30 16:30 .hidden1
-rw-rw-r-- 1 vagrant vagrant    0 Apr 30 16:30 .hidden2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 30 17:08 ..hidden3
$ ./hls -1A test
abc
file
File2
file3
folder1
Folder2
folder3
.hidden1
.hidden2
..hidden3
$
```

### :white_check_mark: 7. Esrever
Implement the `-r` option.
* Usage: `hls [-1aAlr] [FILE]...`

```bash
$ ./hls test
abc file File2 file3 folder1 Folder2 folder3
$ ./hls test -r
folder3  Folder2  folder1  file3  File2  file  abc
$ ./hls test/folder1/ test/folder3/ -r
test/folder3/:
symlink  folder3-1  file2  file1

test/folder1/:
folder1-3  folder1-2  folder1-1
$ 
```

### :white_check_mark: 8. More sorting?
Implement the `-S` option.
Usage: `hls [-1aAlrS] [FILE]...`

```bash
$ ./hls -l test
-rw-rw-r-- 1 vagrant vagrant    0 May 16 12:36 abc
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 File2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file3
drwxrwxr-x 5 vagrant vagrant 4096 Apr 30 15:22 folder1
drwxrwxr-x 2 vagrant vagrant 4096 May  7 15:36 Folder2
drwxrwxr-x 3 vagrant vagrant 4096 May  7 15:37 folder3
$ ./hls -lS test
drwxrwxr-x 5 vagrant vagrant 4096 Apr 30 15:22 folder1
drwxrwxr-x 2 vagrant vagrant 4096 May  7 15:36 Folder2
drwxrwxr-x 3 vagrant vagrant 4096 May  7 15:37 folder3
-rw-rw-r-- 1 vagrant vagrant    0 May 16 12:36 abc
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 File2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file3
$ ./hls -lSr test
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file3
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 File2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file
-rw-rw-r-- 1 vagrant vagrant    0 May 16 12:36 abc
drwxrwxr-x 3 vagrant vagrant 4096 May  7 15:37 folder3
drwxrwxr-x 2 vagrant vagrant 4096 May  7 15:36 Folder2
drwxrwxr-x 5 vagrant vagrant 4096 Apr 30 15:22 folder1
$
```

### :white_check_mark: 9. I know you like sorting things.
Implement the `-t` option.
* Usage: `hls [-1aAlrSt] [FILE]...`

```bash
$ ./hls -l test
-rw-rw-r-- 1 vagrant vagrant    0 May 16 12:36 abc
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 File2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file3
drwxrwxr-x 5 vagrant vagrant 4096 Apr 30 15:22 folder1
drwxrwxr-x 2 vagrant vagrant 4096 May  7 15:36 Folder2
drwxrwxr-x 3 vagrant vagrant 4096 May  7 15:37 folder3
$ ./hls -lt test
-rw-rw-r-- 1 vagrant vagrant    0 May 16 12:36 abc
drwxrwxr-x 3 vagrant vagrant 4096 May  7 15:37 folder3
drwxrwxr-x 2 vagrant vagrant 4096 May  7 15:36 Folder2
drwxrwxr-x 5 vagrant vagrant 4096 Apr 30 15:22 folder1
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file3
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 File2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file
$ ./hls -ltr test
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 File2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file3
drwxrwxr-x 5 vagrant vagrant 4096 Apr 30 15:22 folder1
drwxrwxr-x 2 vagrant vagrant 4096 May  7 15:36 Folder2
drwxrwxr-x 3 vagrant vagrant 4096 May  7 15:37 folder3
-rw-rw-r-- 1 vagrant vagrant    0 May 16 12:36 abc
$
```

### :white_check_mark: 10. The Juggernaut
Implement the `-R` option.
* Usage: `hls [-1aAlrStR] [FILE]...`

```bash
$ ./hls -R test
test:
abc  ABC  file  File2  file3  folder1  Folder2  folder3

test/folder1:
folder1-1  folder1-2  folder1-3

test/folder1/folder1-1:
file1  file2  file3

test/folder1/folder1-2:
file1

test/folder1/folder1-3:
file1  file2

test/Folder2:
file1  file2

test/folder3:
file1  file2  folder3-1  symlink

test/folder3/folder3-1:
file1  folder3-1-1

test/folder3/folder3-1/folder3-1-1:
file1  file2
$ ./hls -Rl test
test:
-rw-rw-r-- 1 vagrant vagrant    0 May 16 12:36 abc
-rw-rw-r-- 1 vagrant vagrant    0 May  8 00:19 ABC
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 File2
-rw-rw-r-- 1 vagrant vagrant    0 Apr 18 14:04 file3
drwxrwxr-x 5 vagrant vagrant 4096 Apr 30 15:22 folder1
drwxrwxr-x 2 vagrant vagrant 4096 May  7 15:36 Folder2
drwxrwxr-x 3 vagrant vagrant 4096 May  7 15:37 folder3

test/folder1:
drwxrwxr-x 2 vagrant vagrant 4096 Apr 30 14:59 folder1-1
drwxrwxr-x 2 vagrant vagrant 4096 Apr 30 14:59 folder1-2
drwxrwxr-x 2 vagrant vagrant 4096 Apr 30 14:59 folder1-3

test/folder1/folder1-1:
-rw-rw-r-- 1 vagrant vagrant 0 Apr 30 14:59 file1
-rw-rw-r-- 1 vagrant vagrant 0 Apr 30 14:59 file2
-rw-rw-r-- 1 vagrant vagrant 0 Apr 30 14:59 file3

test/folder1/folder1-2:
-rw-rw-r-- 1 vagrant vagrant 0 Apr 30 14:59 file1

test/folder1/folder1-3:
-rw-rw-r-- 1 vagrant vagrant 0 Apr 30 14:59 file1
-rw-rw-r-- 1 vagrant vagrant 0 Apr 30 14:59 file2

test/Folder2:
-rw-rw-r-- 1 vagrant vagrant 0 Apr 30 14:59 file1
-rw-rw-r-- 1 vagrant vagrant 0 May  7 15:36 file2

test/folder3:
-rw-rw-r-- 1 vagrant vagrant    0 Apr 30 15:03 file1
-rw-rw-r-- 1 vagrant vagrant    0 Apr 30 15:03 file2
drwxrwxr-x 3 vagrant vagrant 4096 Apr 30 15:22 folder3-1
lrwxrwxrwx 1 vagrant vagrant    5 May  8 00:01 symlink -> file2

test/folder3/folder3-1:
-rw-rw-r-- 1 vagrant vagrant    0 Apr 30 15:03 file1
drwxrwxr-x 2 vagrant vagrant 4096 Apr 30 15:03 folder3-1-1

test/folder3/folder3-1/folder3-1-1:
-rw-rw-r-- 1 vagrant vagrant 0 Apr 30 15:03 file1
-rw-rw-r-- 1 vagrant vagrant 0 Apr 30 15:03 file2
$ 
```

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
