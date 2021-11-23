# (360) 0x03. Python - /proc filesystem
Specializations > System programming & Algorithm > Linux Programming

### Project author
Alexandre Gautier

### Assignment dates
01-12-2021 to 01-22-2021

### Description
Introduction to using the `/proc` filesystem to explore the memory footprint of a running process.

### Requirements
* All your modules should have documentation (`python3 -c 'print(__import__("my_module").__doc__)'`)
* All your classes should have documentation (`python3 -c 'print(__import__("my_module").MyClass.__doc__)'`)
* All your functions (inside and outside a class) should have documentation (`python3 -c 'print(__import__("my_module").my_function.__doc__)'` and `python3 -c 'print(__import__("my_module").MyClass.my_function.__doc__)')`


### Provided file(s)

---

## Mandatory Tasks

### :white_check_mark: 0. Hack the VM
Write a script that finds a string in the heap of a running process, and replaces it.

* Usage: `read_write_heap.py pid search_string replace_string`
    * where `pid` is the pid of the running process
    * and strings are ASCII
* The script should look only in the heap of the process
* Output: you can print whatever you think is interesting
* On usage error, print an error message on `stdout` and exit with status code 1

File(s): [`read_write_heap.py`](./read_write_heap.py)

### :white_large_square: 1. Blog post #1
Write a blog post about the `/proc` filesystem and the `/proc/maps` and `/proc/mem` files.

Try to explain how to parse the `/proc/maps` file in order to read the virtual memory.

Your posts should have examples and at least one picture, at the top. Publish your blog post on Medium or LinkedIn, and share it at least on Twitter and LinkedIn.

<!--
https://www.linkedin.com/pulse/using-procmaps-procmem-linux-view-memory-running-process-pomeroy
https://www.linkedin.com/posts/activity-6758106225340231680-voI7
-->

## Advanced Tasks

### :white_large_square: 2. Blog post #2
Write a blog post about the virtual memory.

How does the virtual memory map into the RAM?

Try to go as deep as possible in your explanation, and try to explain how the Kernel handles it.

Your posts should have examples and at least one picture, at the top. Publish your blog post on Medium or LinkedIn, and share it at least on Twitter and LinkedIn.

<!--
https://www.linkedin.com/pulse/using-procmaps-procmem-linux-view-memory-running-process-pomeroy
https://www.linkedin.com/posts/activity-6758106225340231680-voI7
-->

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
