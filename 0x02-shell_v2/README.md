# (327) 0x02. C - Shell v2
Specializations > System programming & Algorithm > Linux Programming

### Project author
Alexandre Gautier

### Assignment dates
12-15-2020 to 01-12-2021

### Description
Revisiting (235) 0x16. C - Simple Shell to build a second clone of `dash` / Ubuntu `sh`, now with logical operators, piping, and input/output redirections.

### Requirements
* Allowed Functions and System Calls
 * `access`
 * `chdir`
 * `close`
 * `execve`
 * `fork`
 * `stat` (__xstat)
 * `lstat` (__lxstat)
 * `fstat` (__fxstat)
 * `kill`
 * `open`
 * `read`
 * `signal`
 * `wait`
 * `waitpid`
 * `wait3`
 * `wait4`
 * `write`
 * `_exit`
 * `dup`
 * `dup2`
 * `pipe`
 * `unlink`
 * `time`
 * `gethostname`
 * `geteuid`
 * `sigaction`
 * `sigemptyset`
 * `select`
 * `getpid`
 * `__errno_location` (errno macros)
 * `closedir`
 * `exit`
 * `free`
 * `getcwd`
 * `getline`
 * `malloc`
 * `opendir`
 * `perror`
 * `readdir`
 * `strtok`
 * `localtime`
 * `getpwuid`
 * `isatty`
 * `printf`
 * `fflush`
 * `fprintf`
 * `vfprintf`
 * `sprintf`
* Error printing should mimic `sh`:
```bash
$ echo "qwerty" | /bin/sh
/bin/sh: 1: qwerty: not found
$ echo "qwerty" | /bin/../bin/sh
/bin/../bin/sh: 1: qwerty: not found
$
```

### Provided file(s)

### Note
This project can be found in a separate repository: [`shell_v2`](https://github.com/allelomorph/shell_v2). This README is to provide the project profile separate from the README originally included with the project itself.

---

## Mandatory Tasks

### :white_check_mark: 0. README, man, AUTHORS
* Write a `README`
* Write a `man` for your shell.
* You should have an `AUTHORS.md` file at the root of your repository, listing all individuals having contributed content to the repository. Format, see [Docker](https://github.com/moby/moby/blob/master/AUTHORS)

File(s): [`README.md`](./README.md) [`man_1_hsh`](./man_1_hsh) [`AUTHORS.md`](./AUTHORS.md)

### :white_check_mark: 1. Betty would be proud
Write a beautiful code that passes the Betty checks

Be aware that every single C source file (`*.c` and `*.h`) inside your repository will be checked

Compiled: `make all`

### :white_large_square: 2. hsh 1.0
Write a UNIX command line interpreter.

* Usage: `hsh`

Your shell must:

* Display a prompt and wait for the user to type a command. A command line always ends with a new line.
* The prompt is displayed again each time a command has been executed.
* If an executable cannot be found, print an error message and display the prompt again.
* Handle errors.
* You have to handle the “end of file” condition (Ctrl+D)
* Handle command lines with arguments
* Handle and parse the `PATH` environment variable

Builtins

* Implement the built-in command `exit`:
    * Your `exit` built-in must behave exactly like `sh`‘s
    * You have to handle arguments

* Implement the built-in command `cd`:
    * Changes the current directory of the process.
    * Command syntax: `cd [path]`
    * If no argument is given to `cd` the command must be interpreted like `cd $HOME`
    * You have to handle the command `cd -`
    * You have to update the environment variable `PWD` when you change directory

* Implement the built-in command env:
    * Prints the current environment

You don’t have to:

* handle special characters : `"`, `'`, `\``, `\`, `*`, `&`, `#`
* be able to move the cursor
* handle pipes and redirections

Compiled: `make all`

### :white_check_mark: 3. hsh 1.1
hsh 1.0 +

* Handle the right stream redirection `>`
* Usage: `command > output_file`

Compiled: `make all`

### :white_check_mark: 5. hsh 1.2
hsh 1.1 +

* Handle the double right stream redirection `>>`
* Usage: `command >> output_file`
* You don’t have to handle the syntax: `>> output_file command`

Compiled: `make all`

### :white_check_mark: 6. hsh 1.3
hsh 1.2 +

* Handle the left stream redirection `<`
* Usage: `command < input_file`

Compiled: `make all`

### :white_check_mark: 7. hsh 1.4
hsh 1.3 +

* Handle the double left stream redirection `<<` (also known as `heredoc`)
* Usage: `command << delimiter`

Compiled: `make all`

### :white_check_mark: 8. hsh 1.5
hsh 1.4 +

* Handle the pipe stream redirection `|`
* Usage: `command1 | command2`

Compiled: `make all`

### :white_check_mark: 9. hsh 1.6
hsh 1.5 +

* Handle the commands separator `;`

Compiled: `make all`

### :white_check_mark: 10. hsh 1.7
hsh 1.6 +

* Handle the `&&` logical operator

Compiled: `make all`

### :white_check_mark: 11. hsh 1.8
hsh 1.7 +

    Handle the `||` logical operator

Compiled: `make all`

### :white_check_mark: 12. hsh 2.0
hsh 1.8 +

Implement the `setenv` and `unsetenv` builtin commands

    `setenv`
        Initialize a new environment variable, or modify an existing one
        Command syntax: `setenv VARIABLE VALUE`
        Should print something on stderr on failure
    `unsetenv`
        Remove a environment variable
        Command syntax: `unsetenv VARIABLE`
        Should print something on stderr on failure

Compiled: `make all`

## Advanced Tasks

### :white_large_square: 4. hsh 1.1.1
hsh 1.1 +

* Handle the right stream redirection `>`
* Usage: `command n> output_file`
    * Where `n` is the file descriptor of the file to redirect the output from
    * `n` is a number between 0 and 9

Compiled: `make all`

### :white_large_square: 13. Ctrl + C
hsh 1.0 +

    Handle Ctrl+C: your shell should not quit when the user inputs `^C`
    Hint: `man 2 signal`

Compiled: `make all`

### :white_large_square: 14. Aliases
hsh 1.0 +

    Implement the `alias` builtin command
    Usage: `alias [name[='value'] ...]`
        `alias`: Prints a list of all aliases, one per line, in the form `name='value'`
        `alias name [name2 ...]`: Prints the aliases `name`, `name2`, etc 1 per line, in the form `name='value'`
        `alias name='value' [...]`: Defines an alias for each `name` whose `value` is given. If `name` is already an alias, replaces its value with `value`

Compiled: `make all`

### :white_large_square: 15. $VARIABLES
hsh 1.0 +

    Handle variables replacement
    Handle the `$?` variable
    Handle the `$$` variable

Compiled: `make all`

### :white_check_mark: 16. #Comments
hsh 1.0 +

    Handle comments (`#`)

Compiled: `make all`

### :white_check_mark: 17. Help !
hsh 1.0 +

    Implement the `help` builtin command
    Usage: `help [BUILTIN]`

Compiled: `make all`

### :white_large_square: 18. Move on
hsh 1.0 +

    Implement the `history` builtin command, without any argument
    The `history` builtin displays the history list, one command by line, preceded with line numbers (starting at 0)
    On `exit`, write the entire history, without line numbers, to a file named `.hsh_history` in the directory `$HOME`
    When the shell starts, read the file `.hsh_history` in the directory `$HOME` if it exists, and set the first line number of the current session to the total number of lines in the file modulo 4096

Compiled: `make all`

### :white_check_mark: 19. Try to run this!
hsh 1.0 +

    Usage: `hsh [filename]`
    Your shell can take a file as a command line argument
    The file contains all the commands that your shell should run before exiting
    The file should contain one command per line
    In this mode, the shell should not print a prompt and should not read from `stdin`

Compiled: `make all`

### :white_large_square: 20. . .hshrc
hsh 1.0 +

    At startup, your shell must open the file `.hshrc` located in the `$HOME` directory, and execute it
    Since this task requires only hsh 1.0, there won’t be any advanced content in `.hshrc` file during the correction (no aliases, no redirections, …)

Compiled: `make all`

### :white_check_mark: 21. $PS1
hsh 1.0 +

    When executing interactively, your shell must displays the primary prompt `PS1` when it is ready to read a command
    The `PS1` variable can be customized by inserting a number of backslash-escaped special characters
    You must handle the following backslash-escaped characters:
        `\d`: The date in “Weekday Month Date” format (e.g., “Tue Feb 05”, “Sat Jul 26”)
        `\H`: The hostname
        `\s`: The name of the shell
        `\u`: The username of the current user
        `\w`: The current working directory, with $HOME abbreviated with a tilde (`~`)

Compiled: `make all`

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
