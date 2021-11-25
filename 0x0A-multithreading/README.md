# (442) 0x0A. Multithreading
Specializations > System programming & Algorithm > Linux Programming

### Project author
Alexandre Gautier

### Assignment dates
05-10-2021 to 05-18-2021

### Description
Introduction to the `pthread` library and concepts like concurrency vs parallelism, mutual exclusion, and race conditions.

### Requirements
Global and static variables are allowed for these tasks.

### Provided file(s)
* definitions of `pixel_t`, `img_t`, `kernel_t`, `blur_portion_t`, `task_status_t`, and `task_t` for [`multithreading.h`](./multithreading.h)
* [car_blurred_full.ppm](./images/car_blurred_full.ppm) [car_blurred.ppm](./images/car_blurred.ppm) [car.ppm](./images/car.ppm) [kernel_0.knl](./images/kernel_0.knl) [small_blurred.ppm](./images/small_blurred.ppm) [small.ppm](./images/small.ppm)
* [list.h](./list.h) [list.c](./list.c)
* [`0-main.c`](./tests/0-main.c) [`1-main.c`](./tests/1-main.c) [`10-main.c`](./tests/10-main.c) [`1-main.c`](./tests/11-main.c) [`20-main.c`](./tests/0-main.c) [`21-main.c`](./tests/21-main.c) [`22-main.c`](./tests/22-main.c)

---

## Mandatory Tasks

### :white_check_mark: 0. Thread entry point
Write a function that will serve as the entry point to a new thread.

* Prototype: `void *thread_entry(void *arg);`, where:
    * `arg` holds the address of a string that must be printed and followed by a new line.
* Make sure to respect the order in which strings are printed out:
```
Holberton School
Created thread ID -> nnn...
C is fun
```

File(s): [`0-thread_entry.c`](./0-thread_entry.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic -g3 0-main.c 0-thread_entry.c -o 0-thread_entry -pthread`

### :white_check_mark: 1. Thread Logger
Write a function that uses the `printf` family to print out a given formatted string.

* Prototype: `int tprintf(char const *format, ...);`
* The output must be preceded by the calling thread ID:
```
[nnn...] Hello from thread
[nnn...] Hello from main thread
```

File(s): [`1-tprintf.c`](./1-tprintf.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic -g3 1-main.c 1-tprintf.c -o 1-tprintf -pthread`

### :white_check_mark: 2. Blur portion of an image
Write a method that blurs a portion of an image using a [Gaussian Blur](https://www.youtube.com/watch?v=C_zFhWdM4ic).

* Prototype: `void blur_portion(blur_portion_t const *portion);`, where:
    * `portion` points to a data structure described in the project requirements

File(s): [`10-blur_portion.c`](./10-blur_portion.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic -g3 10-main.c 10-blur_portion.c -o 10-blur_portion`

### :white_check_mark: 3. Blur entire image
Write a method that blurs the entirety of an image using a Gaussian Blur.

* Prototype: `void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel);`, where:
    * `img_blur` holds the address at which the blured image is stored,
    * `img` points to the source image,
    * `kernel` points to the convolution kernel to use
* You will have to use threads in order to blur the full image.
* Each thread should blur a portion of the image.
* You may use up to 16 threads. Keep in mind that too many threads can make your program inefficient.
* You are free to divide the source image in portions of your choice.
* The main goal of this task is to demonstrate that using threads make the process faster.

File(s): [`11-blur_image.c`](./11-blur_image.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic -g3 11-main.c 11-blur_image.c 10-blur_portion.c -o 11-blur_image -pthread`

### :white_check_mark: 4. Thread logger v2
Write a function that uses the `printf` family to print out a given formatted string.

This time, you will also have to use a mutex to avoid race conditions. As you know, a mutex needs to be initialized, and destroyed, but we don’t want to have to do that ourselves in our `main.c` program. Please read about [Constructors and Destructors with GCC]() and it should all make sense :).

* Prototype: `int tprintf(char const *format, ...);`
* The output must be preceded by the calling thread ID
* Tip: You are allowed to use global variables

File(s): [`20-tprintf.c`](./20-tprintf.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic -g3 20-main.c 20-tprintf.c -o 20-tprintf -pthread`

### :white_check_mark: 5. Number to prime factors
Write a function that factorizes a number into a list of prime factors.

* Prototype: `list_t *prime_factors(char const *s);`, where
    * `s` is the string representation of the number to factorize.
    * This number will be positive and fit into an `unsigned long`
* NOTE: This task does not require multithreading, and will not be linked to the `pthread` library.

File(s): [`21-prime_factors.c`](./21-prime_factors.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic -g3 21-main.c 21-prime_factors.c list.c -o 21-prime_factors`

### :white_check_mark: 6. Number to prime factors v2
The goal of this task is to reproduce the previous task, and to improve it using a thread pool. The tread pool will have for purpose to execute a list of tasks.

For this task, you will have to write 3 functions (all in the same file):

* Create task
    * Prototype: `task_t *create_task(task_entry_t entry, void *param);`, where
        * `entry` is a pointer to the entry function of the task,
        * `param` is the parameter that will later be passed to the entry function
    * This function must return a pointer to the created task structure

* Destroy task
    * Prototype: `void destroy_task(task_t *task);`, where
        * `task` is a pointer to the task to destroy

* Execute task list - Thread entry
    * Prototype: `void *exec_tasks(list_t const *tasks);`, where
        * `tasks` is a pointer to the list of tasks to be executed
    * This function serves as a thread entry
    * This function can safely return `NULL` as its return value will not be retrieved
    * This function must go through the list of tasks and execute them, but there’s a challenge:
        * Multiple thread will be going through the list of tasks, and a task must only be executed once
    * You must use `tprintf` to print when a task is started, and completed

File(s): [`22-prime_factors.c`](./22-prime_factors.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic -g3 22-main.c 22-prime_factors.c 21-prime_factors.c list.c 20-tprintf.c -o 22-prime_factors -pthread`

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
