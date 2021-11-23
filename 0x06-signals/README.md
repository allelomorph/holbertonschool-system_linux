# (362) 0x06. C - Signals
Specializations > System programming & Algorithm > Linux Programming

### Project author
Alexandre Gautier

### Assignment dates
02-15-2021 to 02-23-2021

### Description
Introduction to Unix signals: sending, listening, and catching.

### Requirements

### Provided file(s)
* [`0-main.c`](./tests/0-main.c) [`1-main.c`](./tests/1-main.c) [`2-main.c`](./tests/2-main.c) [`3-main.c`](./tests/3-main.c) [`4-main.c`](./tests/4-main.c) [`7-main.c`](./tests/7-main.c) [`8-main.c`](./tests/8-main.c) [`9-main.sh`](./tests/9-main.sh) [`10-main.c`](./tests/10-main.c)
* [`100-main.c`](./tests/100-main.c) [`101-main.c`](./tests/101-main.c) [`102-main.c`](./tests/102-main.c) [`103-main.c`](./tests/103-main.c) [`104-main.c`](./tests/104-main.c)

---

## Mandatory Tasks

### :white_check_mark: 0. Handle signal
Write a function that set a handler for the signal `SIGINT`

* Prototype: `int handle_signal(void);`
* Your function must return 0 on success, or -1 on error
* The program should print `Gotcha! [<signum>]` followed by a new line, every time Control-C is pressed
    * where `<signum>` must be replaced with the signal number that was caught
* `sigaction`(2) is not allowed

File(s): [`0-handle_signal.c`](./0-handle_signal.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 0-main.c 0-handle_signal.c -o 0-handle_signal`

### :white_check_mark: 1. Current handler - signal
Write a function that retrieves the current handler of the signal `SIGINT`

* Prototype: `void (*current_handler_signal(void))(int);`
* Your function returns a pointer to the current handler of `SIGINT`, or `NULL` on failure
* You are not allowed to use `sigaction`(2)
* The handler must be unchanged after calling your function

File(s): [`1-current_handler_signal.c`](./1-current_handler_signal.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 1-main.c 1-set_print_hello.c 1-current_handler_signal.c -o 1-current_handler_signal`

### :white_check_mark: 2. Gotta catch them all
Write a function that set a handler for the signal `SIGINT`

* Prototype: `int handle_sigaction(void);`
* Your function must return 0 on success, or -1 on error
* The program should print `Gotcha! [<signum>]` followed by a new line, every time Control-C is pressed
    * where `<signum>` must be replaced with the signal number that was caught
* `signal`(2) is not allowed

File(s): [`2-handle_sigaction.c`](./2-handle_sigaction.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 2-main.c 2-handle_sigaction.c -o 2-handle_sigaction`

### :white_check_mark: 3. Current handler - sigaction
Write a function that retrieves the current handler of the signal `SIGINT`

* Prototype: `void (*current_handler_sigaction(void))(int);`
* Your function returns a pointer to the current handler of `SIGINT`, or `NULL` on failure
* You have to use the function `sigaction` (`signal` is not allowed)
* The handler must be unchanged after calling your function

File(s): [`3-current_handler_sigaction.c`](./3-current_handler_sigaction.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 3-main.c 3-set_print_hello.c 3-current_handler_sigaction.c -o 3-current_handler_sigaction`

### :white_check_mark: 4. Who said that?!
Write a function that defines a handler for the signal `SIGQUIT` (Control-\ in a shell)

* Prototype: `int trace_signal_sender(void);`
* The handler must print `SIGQUIT` sent by `<pid>` each time a `SIGQUIT` (and only a `SIGQUIT`) is caught
    * Where `<pid>` must be replaced by the PID of the process that sent the signal
* Your function must return 0 on success, or -1 on error

File(s): [`4-trace_signal_sender.c`](./4-trace_signal_sender.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 4-main.c 4-trace_signal_sender.c -o 4-trace_signal_sender`

### :white_check_mark: 5. Description
Write a program that prints a description of a given signal

* Usage: `./describe <signum>`
    * Where `<signum>` is the signal number to be described
    * If the number of arguments is not correct, your program must print `Usage: %s <signum>` (where `%s` is `argv[0]`), followed by a new line, and exit with `EXIT_FAILURE`
* You’re not allowed to have more than 1 function in your file
* You’re not allowed to have more than 12 lines in your function
* You can assume that if a parameter is given, it will be a number

File(s): [`5-signal_describe.c`](./5-signal_describe.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 5-signal_describe.c -o 5-signal_describe`

### :white_check_mark: 6. Catch a single time
Write a program that sets a handler for the signal SIGINT, and exits right after the signal is received and handled

* Your program does not take any argument
* Your program should suspend indefinitely until a signal is received
* When a `SIGINT` is received, you must print `Caught %d` (where `%d` must be replaced by the signal number), followed by a new line
* After the first `SIGINT` is received, your program must:
    * Print `Signal received`, followed by a new line
    * Exit with `EXIT_SUCCESS`
* You are not allowed to use the functions `exit`, `sleep` or `_exit`
* You are not allowed to use any kind of loop (`while`, `for`, `do`/`while`)

File(s): [`6-suspend.c`](./6-suspend.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 6-suspend.c -o 6-suspend`

### :white_check_mark: 7. Sending a signal
Write a program that sends the signal `SIGINT` to a process, given its PID

* Usage: `signal_send <pid>`
    * Where `<pid>` is the PID of the process to send a signal to
    * If the number of arguments is not correct, your program must print `Usage: %s <pid>` (where `%s` is `argv[0]`), followed by a new line, and exit with `EXIT_FAILURE`
    * `<pid>` won’t be 0
* Your program must return `EXIT_SUCCESS` on success, or `EXIT_FAILURE`
* You’re not allowed to have more than 1 function in your file

File(s): [`7-signal_send.c`](./7-signal_send.c)\
Compiled:
* Test: `gcc -Wall -Wextra -Werror -pedantic 7-main.c -o 7-wait_for_it`
* `gcc -Wall -Wextra -Werror -pedantic 7-signal_send.c -o 7-signal_send`

### :white_check_mark: 8. Sending a signal in a shell
Write a `sh` script that sends the signal `SIGQUIT` to a process, given its PID

* Usage: `signal_send.sh <pid>`
    * Where `<pid>` is the PID of the process to send a signal to
    * If the number of arguments is not correct, your program must print `Usage: %s <pid>` (where `%s` is `argv[0]`), followed by a new line, and exit with 1

File(s): [`8-signal_send.sh`](./8-signal_send.sh)\
Compiled: Test: `gcc -Wall -Wextra -Werror -pedantic 8-main.c -o 8-wait_for_it`

### :white_check_mark: 9. Catch with sh
Write a sh script that set a handler for the signals `SIGABRT`, `SIGIO` and `SIGTERM`

* The script must print `Nope` followed by a new line, each time a `SIGABRT`, `SIGIO` or `SIGTERM` is caught
* Your script must contain a shebang
* You’re not allowed to have more than 2 lines in your script
* You’re not allowed to have more than 1 instruction per line

File(s): [`9-handle_signal.sh`](./9-handle_signal.sh)

### :white_check_mark: 10. Does it exist?
Write a function that tests if a process exists, given its PID

* Prototype: `int pid_exist(pid_t pid);`
* Your function must return 1 if the process with the PID `pid` exists, or 0 otherwise
* You’re not allowed to have more than 1 function in your file
* You’re not allowed to have more than 1 line in your function
* You’re not allowed to include more than 2 headers in your file
* You’re not allowed to include your header file `signals.h`
* You’re not allowed to use the function `getpgid`

File(s): [`10-pid_exist.c`](./10-pid_exist.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 10-main.c 10-pid_exist.c -o 10-pid_exist`

### :white_large_square: 11. Blog post
Write a blog post about signals

Your blog post should cover the following:

* What is a signal
* Why do they exist
* When are they delivered and by whom
* What are the default actions of signals
* What happens to a process when it receives a signal without handling it
* What happens to a process when it receives a signal and handles it

Your posts should have examples and at least one picture, at the top. Publish your blog post on Medium or LinkedIn, and share it at least on LinkedIn.

<!--
https://www.linkedin.com/pulse/signals-unix-based-systems-samuel-pomeroy
https://www.linkedin.com/posts/activity-6769885777158660096-Fx0G
-->

## Advanced Tasks

### :white_check_mark: 12. I'm saying it's unkillable.
Write a function that sets up a single handler for all the signals

* Prototype: `void all_in_one(void);`
* You’re not allowed to have more than 2 functions in your file
* You’re not allowed to have more than 10 lines in your functions
* You’re not allowed to use `signal`(2)
* In your handler, you must use the function `psiginfo` to print information about the signal received 

File(s): [`100-all_in_one.c`](./100-all_in_one.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 100-main.c 100-all_in_one.c -o 100-all_in_one`

### :white_check_mark: 13. Sigset
Write a function that initializes a sigset

* Prototype: `int sigset_init(sigset_t *set, int *signals);`
* Where `set` is a pointer to the signal set to initialize
* And `signals` is a 0-terminated array of int, each one being a signal number
* Your function must return 0 on success, or -1 on error

File(s): [`101-sigset_init.c`](./101-sigset_init.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 101-main.c 101-sigset_init.c -o 101-sigset_init`

### :white_check_mark: 14. Block signals
Write a function that block a given set of signals from being delivered to the current process

* Prototype: `int signals_block(int *signals);`
* Where `signals` is a 0-terminated array of `int`, each one being a signal to block
* Your function must return 0 on success, or -1 on error
* You’re not allowed to use either `signal`(2) nor `sigaction`(2)

File(s): [`102-signals_block.c`](./102-signals_block.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 102-main.c 102-signals_block.c -o 102-signals_block`

### :white_check_mark: 15. Unblock signals
Write a function that unblock a given set of signals from being delivered to the current process

* Prototype: `int signals_unblock(int *signals);`
* Where `signals` is a 0-terminated array of `int`, each one being a signal to unblock
* Your function must return 0 on success, or -1 on error
* You’re not allowed to use either `signal`(2) nor `sigaction`(2)

File(s): [`103-signals_unblock.c`](./103-signals_unblock.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 103-main.c 103-signals_unblock.c _signals_block.c -o 103-signals_unblock`

### :white_check_mark: 16. Handle pending signals
Write a function that sets up a handler for all the pending signals of the current process

* Prototype: `int handle_pending(void (*handler)(int));`
* Where `handler` is a pointer to the handler function to set for the pending signals
* Your function must return 0 on success, or -1 on error
* You’re not allowed to use `signal`(2)

File(s): [`104-handle_pending.c`](./104-handle_pending.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic 104-main.c 104-handle_pending.c _signals_block.c _signals_unblock.c -o 104-handle_pending`

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
