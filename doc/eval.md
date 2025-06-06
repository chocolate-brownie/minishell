# Evaluation

``valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell``

### Commands with absolute path

``/bin/ls -l /home/yourusername/Documents``
``/usr/bin/pwd``
``/usr/bin/grep "main" /home/yourusername/projects/minishell/src/main.c``
``/usr/bin/env``
``/bin/echo "Hello, minishell!" > /tmp/hello.txt``

### How many global variables? why? Give a concrete example of why it feels mandatory or logical.

- We use one main global variable, `g_signal`. It's there to catch the Ctrl+C signal (SIGINT) so the shell can react properly, like showing a new prompt, without crashing. Signal handlers often need a global variable to communicate that a signal occurred.

### Echo

```
echo
echo hello
echo this is a test
echo \"  spaced out  \"
echo -n no newline
echo \"-n\" literally
echo \"a backslash: \\\\ and a quote: \\\"\"
echo \"\" \"\"
echo start \"middle part\" end
echo -- -n
echo -n -n hello
```

### Exit

```
# Basic cases
exit
exit 0
exit 42
exit 255

# Edge cases and overflow/underflow (behavior might depend on your atol implementation)
exit 256
exit -1
exit 9223372036854775807  # LLONG_MAX
exit -9223372036854775808 # LLONG_MIN
exit 9223372036854775808  # LLONG_MAX + 1
exit -9223372036854775809 # LLONG_MIN - 1

# Invalid numeric arguments
exit foo
exit " "
exit ""
exit --5
exit ++5
exit 1foo
exit foo1

# Too many arguments
exit 1 2
exit 0 foo
exit foo bar
```
---

## 5 Test Commands for Double Quotes (Excluding `$` expansion)

1.  **Meta-characters as Literals:**
    *   Command: `echo "cat < file.txt | grep 'pattern' > output.log"`
    *   Expected Output: `cat < file.txt | grep 'pattern' > output.log`
    *   Purpose: To ensure that redirection symbols (`<`, `>`) and the pipe symbol (`|`) are treated as literal characters within double quotes and do not trigger their special shell functions. The single quotes inside should also remain literal.

2.  **Backslash and Semicolon as Literals:**
    *   Command: `echo "this is a backslash: \\ and this is a semicolon: ;"`
    *   The subject says: "Not interpret unclosed quotes or special characters which are not required by the subject such as \ (backslash) or ; (semicolon)."

    *   So, inside the double-quoted string `"this is a backslash: \\ and this is a semicolon: ;"`, the sequence \\ will be treated by your shell's parser as two literal backslash characters.

3.  **Wildcard as Literal:**
    *   Command: `echo "Files: *.c"`
    *   Expected Output: `Files: *.c`
    *   Purpose: To ensure that the wildcard character `*` is not expanded and is treated as a literal character within double quotes (as wildcard expansion is a bonus feature and not explicitly tied to double quote interpretation itself in the mandatory part).

4.  **Empty String and Spaces Preservation:**
    *   Command: `echo "First: ''  Second: \"\"  Third:    many spaces    "`
    *   Expected Output: `First: ''  Second: ""  Third:    many spaces    `
    *   Purpose: Tests that an empty string can be represented, that single quotes or double quotes *inside* the main double-quoted string are literal, and that multiple spaces are preserved.

5.  **Unclosed Double Quote (Error Handling):**
    *   Command: `echo "This is an unclosed quote`
    *   Expected Minishell Behavior:
        *   Output to stderr: `minishell: syntax error: unclosed quote` (or similar, as per your implementation)
        *   The `echo` command should NOT execute.
        *   Minishell prompt should reappear, ready for a new command.
    *   Purpose: Verifies the "Not interpret unclosed quotes" rule.

---

## 5 Test Commands for Single Quotes

1.  **Test basic meta-character suppression (wildcard):**
    *   Command: `echo 'Hello * World'`
    *   Expected behavior: The shell should print `Hello * World` literally. The `*` should not be interpreted as a wildcard for filename expansion.

2.  **Test suppression of redirection and pipe operators:**
    *   Command: `echo 'this is a pipe | and this is a redirect > file.txt'`
    *   Expected behavior: The shell should print `this is a pipe | and this is a redirect > file.txt` literally. The `|` and `>` should not be interpreted as pipe or output redirection operators.

3.  **Test preservation of whitespace:**
    *   Command: `echo 'word1   word2		word3'`
    *   Expected behavior: The shell should print `word1   word2		word3` with the exact spacing preserved. The entire single-quoted string, including multiple spaces and tabs, should be treated as a single argument to `echo`.

4.  **Test suppression of variable expansion ($):**
    *   Command: `echo 'My current user is $USER and last exit code is $?'`
    *   Expected behavior: The shell should print `My current user is $USER and last exit code is $?` literally. Neither `$USER` nor `$?` should be expanded to their respective values.

5.  **Test suppression of other special characters (e.g., double quotes within single quotes):**
    *   Command: `echo 'This is a "test" with $PATH'`
    *   Expected behavior: The shell should print `This is a "test" with $PATH` literally. The double quotes should be treated as literal characters, and `$PATH` should not be expanded.


## ENV

**Core Requirements from Subject:**

*   `export` with no options: This implies `export VAR=value` for setting/updating and potentially `export` by itself to list variables (referencing `bash`).
*   `env` with no options or arguments: This should print all environment variables.

**Test Steps:**

Here's a sequence of commands you can run in your `minishell` and what to look for:

1.  **Initial Check (Optional but Recommended):**
    *   Command: `env`
    *   Purpose: See the initial set of environment variables your shell starts with. This provides a baseline.

2.  **Create a New Environment Variable:**
    *   Command: `export MY_TEST_VAR="HelloMinishell"`
    *   Purpose: Test creating a brand-new environment variable.
    *   Verification Command: `env`
    *   Expected Output from `env`: You should see `MY_TEST_VAR=HelloMinishell` in the list of environment variables.

3.  **Replace the Value of an Existing Environment Variable:**
    *   Command: `export MY_TEST_VAR="NewValue123"`
    *   Purpose: Test updating the value of a variable created in the previous step.
    *   Verification Command: `env`
    *   Expected Output from `env`: You should see `MY_TEST_VAR=NewValue123`. The old value "HelloMinishell" should be replaced.

4.  **Create Another New Environment Variable:**
    *   Command: `export ANOTHER_VAR="testing_export"`
    *   Purpose: Ensure multiple variables can be added and coexist.
    *   Verification Command: `env`
    *   Expected Output from `env`: You should see both `MY_TEST_VAR=NewValue123` and `ANOTHER_VAR=testing_export`.

5.  **Export a Variable with an Empty Value:**
    *   Command: `export EMPTY_VAR=`
    *   Purpose: Test if `export` can set a variable to an empty string.
    *   Verification Command: `env`
    *   Expected Output from `env`: You should see `EMPTY_VAR=` (the variable name followed by an equals sign and nothing else).

6.  **Test `export` with an Invalid Identifier (Bash Reference):**
    *   Command: `export 1INVALID_VAR="test"`
    *   Purpose: Identifiers typically cannot start with a number.
    *   Expected Output: Your shell should print an error message (e.g., "minishell: export: `1INVALID_VAR': not a valid identifier") and `1INVALID_VAR` should *not* appear in the output of a subsequent `env` command. The exit status of the `export` command itself should be non-zero.
    *   Verification Command: `env` (to confirm `1INVALID_VAR` was not added).

8.  **Test `export` with another Invalid Identifier (e.g., containing `-`):**
    *   Command: `export MY-INVALID-VAR="test2"`
    *   Purpose: Hyphens are often not allowed in environment variable names (underscores are preferred).
    *   Expected Output: Similar to the above, an error message should be printed, `MY-INVALID-VAR` should not be added to the environment, and the `export` command should have a non-zero exit status.
    *   Verification Command: `env`.

### UNSET

**Core Requirements from Subject:**

*   `unset` with no options: This implies `unset VARNAME` to remove environment variables.

1.  **Setup Phase (Ensure Variables Exist):**
    *   Command: `export VAR_TO_KEEP="I should stay"`
    *   Command: `export VAR_TO_UNSET="I will be removed"`
    *   Command: `export ANOTHER_TO_UNSET="Me too"`
    *   Purpose: Create a few environment variables to work with.
    *   Verification Command: `env`
    *   Expected Output from `env`: You should see `VAR_TO_KEEP=I should stay`, `VAR_TO_UNSET=I will be removed`, and `ANOTHER_TO_UNSET=Me too` along with any other existing variables.

2.  **Unset a Single Existing Variable:**
    *   Command: `unset VAR_TO_UNSET`
    *   Purpose: Test the basic functionality of removing one variable.
    *   Verification Command: `env`
    *   Expected Output from `env`:
        *   `VAR_TO_UNSET` should **no longer** be present.
        *   `VAR_TO_KEEP` and `ANOTHER_TO_UNSET` should still be present.

3.  **Unset Multiple Existing Variables in a Single Command:**
    *   Command: `export TEMP_VAR1="temp1"`
    *   Command: `export TEMP_VAR2="temp2"`
    *   Command: `unset TEMP_VAR1 ANOTHER_TO_UNSET`
    *   Purpose: Test if `unset` can handle multiple arguments to remove several variables at once.
    *   Verification Command: `env`
    *   Expected Output from `env`:
        *   `TEMP_VAR1` and `ANOTHER_TO_UNSET` should **no longer** be present.
        *   `VAR_TO_KEEP` and `TEMP_VAR2` should still be present.

4.  **Attempt to Unset a Non-Existent Variable:**
    *   Command: `unset NON_EXISTENT_VAR`
    *   Purpose: Test how `unset` behaves when asked to remove a variable that isn't set.
    *   Verification Command: `env`
    *   Expected Output from `env`: The environment should remain unchanged from the previous step. `unset` should not produce an error for a non-existent variable (this is standard `bash` behavior). The command should succeed with an exit status of 0.

5.  **Attempt to Unset a Variable with an Invalid Identifier:**
    *   Command: `unset 1INVALID_VAR`
    *   Purpose: Test `unset` with an invalid variable name.
    *   Expected Output: Your shell should print an error message (e.g., "minishell: unset: `1INVALID_VAR': not a valid identifier"). The exit status of `unset` should be non-zero.
    *   Verification Command: `env` (the environment should be unchanged).

7.  **Unset All Remaining Test Variables:**
    *   Command: `unset VAR_TO_KEEP TEMP_VAR2`
    *   Purpose: Clean up.
    *   Verification Command: `env`
    *   Expected Output from `env`: `VAR_TO_KEEP` and `TEMP_VAR2` should be gone.

---

### Environment Path

**Test 1: Execute commands without any path (relying on `$PATH`)**

1.  **Prerequisite:** Ensure your `$PATH` is set to something sensible that includes standard command directories (like `/bin`, `/usr/bin`). Your shell should inherit this from its parent environment when it starts.
    *   You can verify this in your `minishell` with: `env | grep PATH`

2.  **Commands to Test:**
    *   `ls`
    *   `wc` (you might pipe some input to it, e.g., `echo "hello" | wc`)
    *   `cat /etc/hosts` (using `cat` as an example, any common command will do)
    *   `pwd` (although this is a built-in, it's good to see it mixed in)
    *   `echo $USER`

3.  **Expected Behavior:**
    *   Each command (`ls`, `wc`, `cat`) should be found and executed correctly. Your shell needs to search through the directories listed in the `$PATH` variable to locate these executables.
    *   `pwd` and `echo` (if built-in) should work as expected regardless of `$PATH` for executables.

**Test 2: Unset `$PATH` and check command execution**

1.  **Unset PATH:**
    *   Command: `unset PATH`

2.  **Verify PATH is Unset:**
    *   Command: `env | grep PATH`
    *   Expected Behavior: The `PATH` variable should no longer appear in the environment.
    *   Command: `echo $PATH`
    *   Expected Behavior: Should print an empty line (or nothing), as `$PATH` is unset and should expand to an empty string.

3.  **Attempt to Execute Commands (that previously worked via `$PATH`):**
    *   Command: `ls`
    *   Command: `wc`
    *   Command: `cat /etc/hosts`

4.  **Expected Behavior:**
    *   Your shell should **fail** to find these commands. It should print an error message for each, similar to "minishell: ls: command not found".
    *   The exit status `$?` after each failed command should be non-zero (typically 127 for "command not found").
    *   **Important:** Built-in commands like `echo`, `pwd`, `cd`, `export`, `unset`, `env`, `exit` should *still work* because the shell doesn't need `$PATH` to find its own internal commands.
        *   Test: `echo "Still here"` (should work)
        *   Test: `pwd` (should work)
    *   Commands with an absolute path should also still work:
        *   Test: `/bin/ls` (This should work, as you're providing the full path)

**Test 3: Set `$PATH` to multiple directories and check search order**

This is the trickiest to set up perfectly without creating actual dummy executables, but here's a conceptual approach and a practical one:

**Conceptual Approach (Ideal, but more setup):**

1.  Create two directories in your workspace, say `mybin1` and `mybin2`.
    *   `mkdir mybin1`
    *   `mkdir mybin2`
2.  In `mybin1`, create a simple executable script named `mycmd`:
    *   `echo '#!/bin/sh' > mybin1/mycmd`
    *   `echo 'echo "Executed from mybin1"' >> mybin1/mycmd`
    *   `chmod +x mybin1/mycmd`
3.  In `mybin2`, create another executable script with the *same name* `mycmd` but different output:
    *   `echo '#!/bin/sh' > mybin2/mycmd`
    *   `echo 'echo "Executed from mybin2"' >> mybin2/mycmd`
    *   `chmod +x mybin2/mycmd`
4.  **Set PATH order 1 (mybin1 first):**
    *   Command: `export PATH="./mybin1:./mybin2"` (adjust path if `mybin1`/`mybin2` are not in the current dir)
    *   Verification Command: `env | grep PATH`
    *   Execute Test Command: `mycmd`
    *   Expected Output: `Executed from mybin1`
5.  **Set PATH order 2 (mybin2 first):**
    *   Command: `export PATH="./mybin2:./mybin1"`
    *   Verification Command: `env | grep PATH`
    *   Execute Test Command: `mycmd`
    *   Expected Output: `Executed from mybin2`
6.  **Set PATH with a non-existent directory first:**
    *   Command: `export PATH="./nonexistent:./mybin1"`
    *   Execute Test Command: `mycmd`
    *   Expected Output: `Executed from mybin1` (The shell should skip `./nonexistent` and find it in `./mybin1`)
7.  **Set PATH with only one valid directory containing the command:**
    *   Command: `export PATH="./mybin2:/some/other/standard/path"`
    *   Execute Test Command: `mycmd`
    *   Expected Output: `Executed from mybin2`

**Practical Approach (Using existing commands, less direct for order testing but still useful):**

Since creating dummy executables might be cumbersome for a quick test, you can try to use different versions of standard utilities if they exist in different common `PATH` directories, but this is less reliable for proving *your shell's* search order logic vs. system setup.

A more focused practical test:

1.  **Ensure a known command exists in only one of two specific directories you'll put in PATH.**
    *   This is the hard part without creating your own. Let's assume for a moment you have a unique script or a less common utility in a specific local directory, say `~/custom_utils/my_script`.
2.  **Initial State (Clean up PATH if needed):**
    *   Maybe `unset PATH` first, or `export PATH="/usr/bin:/bin"` (a basic known set).
3.  **Set PATH with your custom directory first:**
    *   Command: `export PATH="~/custom_utils:/usr/bin:/bin"` (replace `~/custom_utils` as needed)
    *   Execute Command: `my_script`
    *   Expected: Your script from `~/custom_utils` runs.
4.  **Set PATH with your custom directory second (or later):**
    *   Command: `export PATH="/usr/bin:/bin:~/custom_utils"`
    *   Execute Command: `my_script`
    *   Expected: Your script from `~/custom_utils` runs (assuming it's not also in `/usr/bin` or `/bin`).
5.  **Set PATH to include a directory where the command *doesn't* exist, followed by one where it *does*:**
    *   Command: `export PATH="/tmp:/usr/bin"`
    *   Execute Command: `ls`
    *   Expected: `ls` from `/usr/bin` (or `/bin` if that's where it first finds it based on a more complex default PATH if /usr/bin is a symlink etc.) should execute. The key is that it *doesn't* fail even though `/tmp` (likely not containing `ls`) is listed first.

**Key things to observe during Test 3:**

*   Your shell should split the `PATH` string by the ':' delimiter.
*   It should try to find the command in each directory listed, from left to right.
*   The first directory in `PATH` that contains an executable matching the command name is the one that should be used.
*   If a directory in `PATH` doesn't exist, it should be silently skipped, and the search should continue to the next directory.

**After Testing:**

*   Remember to restore your `$PATH` to its original or a working value if you unset it or changed it drastically. You might need to exit and restart your `minishell` or manually `export PATH=...` with your system's default.

These tests will give you good confidence in your `PATH` handling and command execution logic!
