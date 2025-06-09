# Critical Bug Fix: Heredoc EOF Handling Race Condition

## Summary
Fixed a critical race condition in heredoc handling when EOF (Ctrl+D) is encountered before the delimiter. This bug caused spurious "No such file or directory" errors while the proper warning message was already being displayed correctly.

## Bug Description
When using heredocs in pipelines and encountering EOF (Ctrl+D) before entering the delimiter, minishell would:
1. ✅ Show the correct warning: `minishell: warning: here-document delimited by end-of-file (wanted 'EOF')`
2. ❌ **Show additional error**: `/tmp/minishell_hd_0: No such file or directory`

**Example command that triggered the bug:**
```bash
echo hello world | cat << EOF
# Press Ctrl+D here instead of typing content and EOF
```

This behavior differed from bash, which only shows the warning and continues execution normally.

## Root Cause Analysis
The issue was a **race condition in pipeline execution**:

1. **Pipeline forks two child processes**: `echo hello world` and `cat << EOF`
2. **Both processes share the same context** with active heredoc files list
3. **First child process finishes first** (`echo`) and calls `free_all(ctx)`
4. **`free_all()` calls `cleanup_all_active_heredocs(ctx)`** - deleting ALL heredoc files
5. **Second child process tries to open heredoc file** (`cat`) but it's already gone
6. **Result**: "No such file or directory" error

## Changes Made

### 1. Fixed Child Process Heredoc Cleanup
**File**: `src/execution/utils/free_all.c`
- **Removed**: `cleanup_all_active_heredocs(ctx)` from `free_all()` function
- **Reason**: Child processes should not clean up shared heredoc resources

### 2. Preserved Main Process Cleanup
**File**: `src/utils/cleanup.c`
- **Kept**: `cleanup_all_active_heredocs(ctx)` in `cleanup_tcontext()`
- **Reason**: Main shell process still properly cleans up heredocs on exit

### 3. Enhanced EOF Close Error Handling
**File**: `src/heredoc/hd_manager.c`
- **Fixed**: Close error condition from `read_status == 0` to `read_status != 0`
- **Fixed**: Main cleanup condition from `read_status != 0` to `read_status < 0`
- **Reason**: EOF completion (status 0) should not trigger error handling

### 4. Improved Error Resilience
**File**: `src/heredoc/hd_active_list_utils.c`
- **Removed**: `set_exit_code()` calls from `add_active_heredoc()` on malloc failures
- **Reason**: Tracking heredoc files is not critical for execution; shouldn't set error codes

## Testing Results

### ✅ Before Fix (Bug Present)
```bash
echo hello world | cat << EOF
# Ctrl+D
minishell: warning: here-document delimited by end-of-file (wanted 'EOF')
/tmp/minishell_hd_0: No such file or directory  # ❌ Spurious error
```

### ✅ After Fix (Bug Resolved)
```bash
echo hello world | cat << EOF
# Ctrl+D
minishell: warning: here-document delimited by end-of-file (wanted 'EOF')
# ✅ No additional error - matches bash behavior exactly
```

### ✅ Regression Testing
- **Normal heredocs**: Still work correctly
- **Complex pipelines**: `echo hello | cat << EOF | echo boom` works properly
- **SIGINT handling**: Ctrl+C still works correctly in heredocs
- **Error cases**: Real errors still properly reported

## Verification
The fix has been verified to:
- ✅ Eliminate the spurious error message
- ✅ Maintain proper bash-compatible warning for EOF in heredocs
- ✅ Preserve all existing heredoc functionality
- ✅ Handle complex pipeline scenarios correctly
- ✅ Maintain proper cleanup on shell exit

## Files Modified
- `src/execution/utils/free_all.c` - Removed child process heredoc cleanup
- `src/heredoc/hd_manager.c` - Fixed EOF and error handling logic
- `src/heredoc/hd_active_list_utils.c` - Improved error resilience

## Impact
This fix resolves a critical user-facing bug that caused confusing error messages when using heredocs in pipelines, bringing minishell's behavior in line with bash standards.
