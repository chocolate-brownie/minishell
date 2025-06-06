### Issue Summary

1.  **Incorrect Parsing of Commands with Leading Empty Variables**: A command starting with an empty variable (e.g., `$EMPTY_VAR echo hi`) would fail to execute. The parser was incorrectly treating the expanded empty string as the command, causing the actual command (`echo`) to be ignored.

### Solution Implementation

1.  **Skipping of Initial Empty Tokens**:
    - The `process_word_token` function in `src/parsing/parser/exec_utils.c` was updated. It now iterates past any initial word tokens that are empty strings. This ensures that the parser correctly identifies the first non-empty word as the command to be executed, aligning its behavior with `bash`.
