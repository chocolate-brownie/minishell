# Minishell

A custom implementation of a command-line interpreter, or "shell," created as a project at École 42. This project is a fundamental exercise in understanding process management, file descriptors, and the overall functionality of a Unix-like shell.

## About The Project

This project's main goal is to create a functional shell in C that can:

  * Display a prompt.
  * Read and parse user input.
  * Execute commands with their arguments.
  * Manage environment variables.
  * Implement I/O redirections and pipes.
  * Handle signals like `Ctrl-C`, `Ctrl-D`, and `Ctrl-\`.

This project was written entirely in C and uses the `readline` library for handling user input.

## Features

### Mandatory Features

  * **Interactive Prompt:** Displays a prompt and waits for user input.
  * **Command History:** Manages command history using `readline`.
  * **Command Execution:** Executes system binaries (like `/bin/ls`) and local executables.
  * **Built-in Commands:**
      * `echo` with the `-n` option.
      * `cd` with relative and absolute paths.
      * `pwd` to display the current working directory.
      * `export` to manage environment variables.
      * `unset` to remove environment variables.
      * `env` to display the environment variables.
      * `exit` to gracefully terminate the shell.
  * **Redirections:**
      * `<` input redirection.
      * `>` output redirection.
      * `<<` here-document.
      * `>>` append output redirection.
  * **Pipes (`|`):** Allows for the output of one command to be the input of another.
  * **Signal Handling:**
      * `Ctrl-C`: Interrupts the current process and displays a new prompt.
      * `Ctrl-D`: Exits the shell.
      * `Ctrl-\`: Does nothing.
  * **Environment Variable Expansion:** Correctly expands variables (e.g., `$USER` or `$PATH`).

## How to Compile and Run

### Prerequisites

You will need the `gcc` compiler and the `readline` library installed on your system. On a standard 42 Linux setup, these should already be available.

### Compilation

1.  Clone the repository:
    ```bash
    git clone [https://github.com/chocolate-brownie/minishell]
    cd minishell
    ```
2.  Compile the project using the Makefile:
    ```bash
    make
    ```

**Question:** What is the name of the final executable that your Makefile will create? The standard is `minishell`, but it's good to be explicit.

### Execution

To run the shell, execute the compiled binary:

```bash
./minishell
```

You should now see the minishell prompt, ready to accept commands.

## Example Usage

Here are a few examples of commands you can run in this minishell:

```sh
# Simple command
> ls -l

# Command with redirection
> cat file1 > file2

# Using a pipe
> cat /dev/urandom | head -c 100 | wc

# Using a built-in
> export HELLO="Hello World!"
> echo $HELLO
Hello World!
> unset HELLO
> echo $HELLO

> exit
```
