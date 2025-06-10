# Fix: Resolve Heredoc Memory Leaks After Command Execution

## Summary
This pull request resolves a memory leak associated with heredocs. The temporary files created for heredocs and their corresponding tracking structures were not being cleaned up after each command, leading to resource leakage for the entire duration of the shell session. The fix introduces a cleanup mechanism that runs after each command pipeline completes.

## Bug Description
When a command involving a heredoc was executed, the memory allocated for tracking the temporary file and the file itself were not released. This leak was cumulative; running multiple commands with heredocs would consume more and more memory, only freeing it when the shell itself was terminated.

This could be consistently reproduced and was identified using `valgrind`.

**Example `valgrind` output showing the leak:**
```
==11945== LEAK SUMMARY:
==11945==    definitely lost: 16 bytes in 1 blocks
==11945==    indirectly lost: 20 bytes in 1 blocks
==11945==      possibly lost: 0 bytes in 0 blocks
==11945==    still reachable: 0 bytes in 0 blocks
==11945==         suppressed: 208,315 bytes in 229 blocks
```

## Root Cause Analysis
The core issue was that the `minishell` process only cleaned up heredoc-related resources upon exiting the main shell loop. There was no mechanism to deallocate these resources after the command that used them had finished executing.

Specifically:
-   When `handle_heredoc` was called, it created a temporary file (e.g., `/tmp/minishell_hd_0`).
-   It then called `add_active_heredoc` to add a `t_hd_temp_file` node to the global `ctx->active_heredocs` list.
-   This list was only ever cleared by `cleanup_all_active_heredocs` when the shell was finally exiting, not between prompts.

## Changes Made

### 1. Implemented Per-Command Heredoc Cleanup
**File**: `src/heredoc/hd_active_list_utils.c`
-   **Added**: A new function `cleanup_command_heredocs(t_exec *exec_list, t_context *ctx)`.
-   **Logic**: This function traverses the execution list (`t_exec`) of the just-completed command. It inspects every redirection, and if it finds a `REDIR_HEREDOC`, it calls `remove_and_unlink_active_heredoc` to free the tracking node and delete the associated temporary file.

### 2. Integrated Cleanup into the Main Loop
**File**: `src/utils/run_minishell.c`
-   **Modified**: The `process_command` function was updated to call `cleanup_command_heredocs` immediately after `execute_pipeline` returns.
-   **Reason**: This ensures that regardless of the pipeline's success or failure, the heredoc resources associated with it are always cleaned up before processing the next command.

### 3. Added Function Declaration
**File**: `includes/minishell.h`
-   **Added**: The function prototype for `cleanup_command_heredocs` was added to make it accessible across the application.

## Testing Results

### ✅ Before Fix (Leak Present)
Running a heredoc command and then another command would show leaks in `valgrind`:
```
valgrind --leak-check=full ./minishell
minishell → ls << EOF
heredoc> EOF
...
minishell → some_other_command
...
==11945== LEAK SUMMARY:
==11945==    definitely lost: 16 bytes in 1 blocks
==11945==    indirectly lost: 20 bytes in 1 blocks
```

### ✅ After Fix (Leak Resolved)
The same sequence of commands now shows no leaks when exiting the shell.
```
valgrind --leak-check=full ./minishell
...
==11850== HEAP SUMMARY:
==11850==     in use at exit: 0 bytes in 0 blocks
==11850==   total heap usage: ...
==11850==
==11850== All heap blocks were freed -- no leaks are possible
```

## Verification
The fix has been verified to:
-   ✅ Completely eliminate the memory leak associated with heredocs.
-   ✅ Ensure temporary files are unlinked promptly after command execution.
-   ✅ Not introduce any regressions in heredoc functionality or overall shell behavior.

## Files Modified
-   `src/heredoc/hd_active_list_utils.c`
-   `includes/minishell.h`
-   `src/utils/run_minishell.c`

## Impact
This fix is crucial for the long-term stability and reliability of the shell. By preventing cumulative memory leaks, it ensures that `minishell` can run indefinitely without consuming ever-increasing system resources.
