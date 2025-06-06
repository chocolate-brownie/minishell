### Issue Summary

This pull request addresses three critical bugs in the minishell's parsing and execution logic:

1.  **Incorrect Redirection Error Message**: When a redirection operator (`<`, `>`, `>>`) was not followed by a filename, the shell incorrectly reported a `syntax error: unclosed quote` instead of the expected `syntax error near unexpected token 'newline'`.
2.  **Duplicate Unclosed Quote Errors**: When a command contained a genuine unclosed quote (e.g., `echo "hello`), the shell would output two separate and confusing error messages.
3.  **Mishandling of Non-Existent Environment Variables**: Referencing an environment variable that was not set (e.g., `$FOO`) caused the shell to fail with a `command not found` error. The expected behavior is for the variable to expand to an empty string, allowing the command to execute without error.

### Solution Implementation

To resolve these issues, the following changes were made across the lexer, parser, and executor:

1.  **Corrected Redirection Parsing**:
    - The logic in `src/parsing/parser/redirs.c` was updated to explicitly check for a `TOKEN_PIPE` or `TOKEN_EOF` after a redirection operator. This ensures that a missing filename is correctly identified as a syntax error, generating the appropriate error message.

2.  **Refined Unclosed Quote Error Handling**:
    - The lexer's main loop in `src/parsing/lexer/lexer.c` was modified to catch the specific `ERR_UNCLOSED_QUOTE` error code. Upon detection, it now prints the correct, single error message and halts further processing, preventing the generation of a second, erroneous message.

3.  **Proper Handling of Non-Existent Variables**:
    - A check was added to the `child_execute_external_command` function in `src/execution/exec/child_process_utils.c`. This check verifies if a command is `NULL` or an empty string (as would be the case with a non-existent variable). If so, the function now exits gracefully with a `0` status code, mirroring the behavior of `bash` and preventing the "command not found" error.
