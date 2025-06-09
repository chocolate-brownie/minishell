# Critical Fix: Resolve Parser Infinite Loop and Memory Corruption

## Summary
This pull request addresses a critical stability issue where providing multiple consecutive redirection operators (e.g., `>>>>`, `<<<<`) would cause the parser to enter an infinite loop, hanging the shell. The fix also resolves a memory corruption bug in the parser's error handling for invalid pipe syntax, which contributed to the shell's instability.

## Bug Description
When a user entered a sequence of redirection tokens, the shell would hang indefinitely. A `Ctrl+C` would break the loop and provide a new prompt, but the shell would be in an unstable state where built-in commands like `exit` and `Ctrl+D` would no longer function. The only way to terminate the process was to suspend it with `Ctrl+Z` and then kill it manually.

**Example command that triggered the bug:**
```bash
minishell → >>>>>>>>>>>>
# The shell hangs here. After Ctrl+C:
minishell → exit
# exit command does not work.
```

## Root Cause Analysis
The instability was caused by two distinct but related bugs in the parser:

1.  **Infinite Loop on Syntax Error**: The primary cause of the hang was that the main parsing loop in `src/parsing/parser/parser.c` did not check if a syntax error had already been flagged. When an error like `>>>>` was encountered, the error flag `ctx->has_syntax_error` was set, but the loop would not terminate. It would attempt to re-process the same invalid tokens endlessly.

2.  **Memory Corruption in Pipe Error Handling**: A separate, severe bug was discovered in `src/parsing/parser/parser_utils.c`. The `invalseg_after_pipe` function, which handles errors like `cmd | |`, was incorrectly using `free_exec_list` to deallocate a single `t_exec` node. This function is designed to traverse a linked list, so calling it on a single node caused it to read uninitialized memory from the `->next` pointer, leading to heap corruption. This corruption is the likely reason `exit` and other functions failed after the initial hang was interrupted.

## Changes Made

### 1. Enforced Parser Termination on Error
**File**: `src/parsing/parser/parser.c`
- **Modified**: The main `while` loop in the `parse_token_loop` function was updated to check for `ctx->has_syntax_error`.
- **Reason**: This ensures that the parser terminates immediately as soon as a syntax error is detected, preventing any possibility of an infinite loop.

### 2. Corrected Heap Corruption Bug
**File**: `src/parsing/parser/parser_utils.c`
- **Fixed**: In the `invalseg_after_pipe` function, the call to `free_exec_list` was replaced with `free_single_exec_node_content` followed by `free`.
- **Reason**: This ensures that a single `t_exec` node is deallocated correctly without reading uninitialized memory, preventing heap corruption and restoring shell stability after a syntax error.

### 3. Refactored Redirection Error Handling
- **Files**: `src/parsing/parser/redirs.c`, `src/heredoc/hd_manager.c`, `src/parsing/parser/redir_utils.c`, `includes/minishell.h`
- **Refactored**: As part of the debugging process, the logic for advancing the token pointer after a redirection syntax error was centralized. This responsibility was moved from utility functions to the primary redirection handling functions (`handle_other_redirs` and `handle_heredoc`), making the parser's state management more robust and predictable.

## Testing Results

### ✅ Before Fix (Bug Present)
```bash
minishell → >>>>>>>>>
# Shell hangs. After Ctrl+C, exit is unresponsive.
# Requires Ctrl+Z and `kill` to terminate.
```

### ✅ After Fix (Bug Resolved)
```bash
minishell → >>>>>>>>>
minishell: syntax error near unexpected token `>>'
minishell → exit
# Shell exits cleanly with status 2.
```

## Verification
The fix has been verified to:
- ✅ Eliminate the infinite loop when parsing invalid redirection sequences.
- ✅ Prevent heap corruption, ensuring the shell remains stable after a syntax error.
- ✅ Allow `exit` and `Ctrl+D` to function correctly after a syntax error is reported.
- ✅ Return the correct exit code (`2`) for syntax errors.

## Files Modified
- `src/parsing/parser/parser.c`
- `src/parsing/parser/parser_utils.c`
- `src/parsing/parser/redirs.c`
- `src/parsing/parser/redir_utils.c`
- `src/heredoc/hd_manager.c`
- `includes/minishell.h`

## Impact
This fix resolves a major stability flaw in the parser, significantly improving its resilience to syntax errors. The shell no longer hangs or crashes on invalid input, leading to a much more robust and reliable user experience.
