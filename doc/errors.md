~~**Memory leaks in Heredoc**~~

==46594== 20 bytes in 1 blocks are indirectly lost in loss record 11 of 69
==46594==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==46594==    by 0x111037: ft_strdup (in /home/mgodawat/Documents/minishell/minishell)
==46594==    by 0x10C97C: add_active_heredoc (in /home/mgodawat/Documents/minishell/minishell)
...
==46594== 36 (16 direct, 20 indirect) bytes in 1 blocks are definitely lost in loss record 17 of 69
==46594==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==46594==    by 0x10C948: add_active_heredoc (in /home/mgodawat/Documents/minishell/minishell)
...

``cleanup_all_active_heredoc(ctx)`` function was missing from the file free_all.c in function ``void free_all(t_context *ctx)``

---

~~**Error with how the lexer passes the `\`**~~
New function has been added for handling backslashes within an unquoted context is now in its own static function, ``process_backslash_unquoted``

Before:

```
minishell → echo \"   spaced out   \"
\   spaced out   \
```

After:

```
minishell → echo \"   spaced out    \"
" spaced out "
```

---

~~**Issue with `exit` command and inline comments**~~

The `exit` command was incorrectly interpreting arguments followed by a `#` (comment marker) as "too many arguments", instead of ignoring the comment as `bash` does. For example, `exit 0 # will exit with 0` would fail.

Additionally, the `ft_exit` function and its helpers in `src/execution/builtins/exit.c` were refactored to meet Norminette requirements. Helper functions (`is_numeric`, `result_loop`, `ft_atol_with_error`, `set_error_message_and_exit_code`) were moved to a new file `src/execution/builtins/exit_utils.c`. The main `ft_exit` function was split into smaller static helpers (`handle_exit_no_args` and `handle_exit_with_args`) to reduce its line count. Prototypes were updated in `includes/minishell.h` and the `Makefile` was adjusted to include the new `exit_utils.c`.

The `ft_exit` function in `src/execution/builtins/exit.c` was modified to check if the second argument (if present) starts with a `#`. If it does, the argument and any subsequent ones are treated as a comment and ignored, allowing the shell to exit with the status code provided by the first argument.

Before:
```
minishell → exit 9223372036854775807 # LLONG_MAX
minishell: exit: too many arguments
```

After:
```
minishell → exit 9223372036854775807 # LLONG_MAX
exit
(minishell terminates with the correct exit code, 255 in this case due to LLONG_MAX being outside 0-255 range and then masked to unsigned char)
```

---

**Try ctrl-\ in a prompt after you wrote some stuff should quit minishell --> RELAUNCH!**

This says it should quit minishell but in bash it doesnt do anything and in our minishell its the same behaviour so I am not sure, what it "should quit minishell" because it doesnt

---

**Double quotes: anything except $**

---

~~**Attempt to Unset a Variable with an Invalid Identifier:**~~
*   Command: `unset 1INVALID_VAR`
*   Purpose: Test `unset` with an invalid variable name.
*   Expected Output: Your shell should print an error message (e.g., "minishell: unset: `1INVALID_VAR': not a valid identifier"). The exit status of `unset` should be non-zero.
*   Verification Command: `env` (the environment should be unchanged).


~~**`echo $USER`**~~


