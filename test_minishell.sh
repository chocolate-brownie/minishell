#!/usr/bin/env bash
#
# test_minishell.sh
#
# Automatically runs all mandatory test cases from eval.md against ./minishell under Valgrind.
# For each test:
#  1. Captures the actual output of the commands run inside minishell.
#  2. Captures Valgrind’s HEAP & LEAK summaries.
#  3. Writes both “Shell Output” and “Valgrind Output” into results/test_results.md.
# If any test command produces memory leaks, the script stops immediately and reports which one failed.
# All logs and the Markdown file are placed in a “results” folder.
#
# 🎯 Usage: ./test_minishell.sh
#
# ⚠️ Prerequisites:
#    - A compiled ./minishell in the same directory.
#    - A suppression file named "readline.supp" in the same directory.
#    - Valgrind and bash installed.
#
# ------------------------------------------------------------------------------

# Create results directory (clean if it already exists)
RESULT_DIR="results"
rm -rf "$RESULT_DIR"
mkdir -p "$RESULT_DIR"

# Markdown output file (inside results)
RESULT_MD="$RESULT_DIR/test_results.md"

# Initialize the Markdown file
cat > "$RESULT_MD" << 'EOF'
# Minishell Test Results

Below are the detailed results for each automated (mandatory) test case, including:

1. **Shell Output**: What the minishell printed when running the test commands.  
2. **Valgrind HEAP & LEAK Summaries**: Memory-leak diagnostics.  

> 🚨 If any test shows a non-zero “definitely lost” value, the script will stop immediately.

---

EOF

# --- Helper Function ----------------------------------------------------------
run_test() {
    local DESC="$1"
    local CMDS="$2"

    # Sanitize the test description into a safe filename fragment for logs
    local SAFE_NAME
    SAFE_NAME=$(echo "$DESC" | sed 's/[^A-Za-z0-9]/_/g')
    local LOGFILE="$RESULT_DIR/valgrind_${SAFE_NAME}.log"

    echo
    echo "📌 Test: $DESC"
    echo "─────────────────────────────────────────────────────────────────────────"

    # 1. Capture minishell’s actual output in CMD_OUTPUT, 
    #    while Valgrind diagnostics go into LOGFILE.
    local CMD_OUTPUT
    CMD_OUTPUT=$( 
        { 
          echo -e "$CMDS"
          echo "exit"
        } | valgrind \
            --leak-check=full \
            --show-leak-kinds=all \
            --suppressions=readline.supp \
            --log-file="$LOGFILE" \
            ./minishell
    )

    # 2. Extract HEAP SUMMARY and LEAK SUMMARY from the Valgrind log
    local HEAP_SUMMARY
    local LEAK_SUMMARY
    HEAP_SUMMARY=$(grep -A5 "HEAP SUMMARY:" "$LOGFILE" || echo "— (no HEAP SUMMARY found)")
    LEAK_SUMMARY=$(grep -A5 "LEAK SUMMARY:" "$LOGFILE" || echo "— (no LEAK SUMMARY found)")

    # 3. Determine PASS / FAIL by checking for "definitely lost: 0 bytes"
    local STATUS
    if grep -q "definitely lost: 0 bytes" "$LOGFILE"; then
        STATUS="PASS ✅"
        echo "✅  No leaks in test: \"$DESC\""
    else
        STATUS="FAIL ❌"
        echo
        echo "❌  Memory leaks detected in test: \"$DESC\""
        echo "   ▶️  Inspect the Valgrind log: $LOGFILE"
        echo "   Stopping further tests."
    fi
    echo "─────────────────────────────────────────────────────────────────────────"

    # 4. Append results to the Markdown file
    {
      echo "## Test: $DESC"
      echo
      echo "**Status:** $STATUS"
      echo
      echo "**Command(s) Sent to Minishell:**"
      echo '```bash'
      echo -e "$CMDS"
      echo '```'
      echo
      echo "**Shell Output:**"
      echo '```'
      if [[ -z "$CMD_OUTPUT" ]]; then
        echo "(no output)"
      else
        echo "$CMD_OUTPUT"
      fi
      echo '```'
      echo
      echo "**Valgrind HEAP SUMMARY:**"
      echo '```'
      echo "$HEAP_SUMMARY"
      echo '```'
      echo
      echo "**Valgrind LEAK SUMMARY:**"
      echo '```'
      echo "$LEAK_SUMMARY"
      echo '```'
      echo
      echo '---'
      echo
    } >> "$RESULT_MD"

    # If failure, exit script immediately
    if [[ "$STATUS" == "FAIL ❌" ]]; then
        exit 1
    fi
}

# --- Main Script --------------------------------------------------------------
echo
echo "🚀 Starting Minishell test suite under Valgrind..."
echo "    Detailed results will be saved to: $RESULT_MD"
echo "==========================================================================="

#
# SECTION 1: Fork & execve basics (4 tests)
#
echo
echo "🔹 Section 1: Fork & execve (4 tests)"
echo "==========================================================================="

run_test "Invalid command (foo → error + prompt)" \
    "foo"

run_test "Absolute path: /bin/ls → should list directory" \
    "/bin/ls"

run_test "Absolute path + flags: /bin/ls -laF" \
    "/bin/ls -laF"

run_test "Absolute path + separated flags: /bin/ls -l -a -F" \
    "/bin/ls -l -a -F"

#
# SECTION 2: Builtins (8 tests)
#
echo
echo "🔹 Section 2: Builtins (8 tests)"
echo "==========================================================================="

run_test "Builtin: exit → should terminate immediately" \
    "exit"

run_test "Builtin: echo \"It works\" (with quotes)" \
    "echo \"It works\""

run_test "Builtin: echo It works (no quotes)" \
    "echo It works"

# cd with absolute path, then /bin/pwd
ABS_PATH="$(pwd)"
run_test "Builtin: cd $ABS_PATH → then /bin/pwd" \
    "cd $ABS_PATH\n/bin/pwd"

# cd with relative path, then /bin/pwd
run_test "Builtin: cd . (relative) → then /bin/pwd" \
    "cd .\n/bin/pwd"

# cd with no args → returns to home, then /bin/pwd
run_test "Builtin: cd (no args → HOME) → then /bin/pwd" \
    "cd\n/bin/pwd"

# cd -, then /bin/pwd
run_test "Builtin: cd - (previous dir) → then /bin/pwd" \
    "cd /\ncd -\n/bin/pwd"

# cd ~ (HOME), then /bin/pwd
run_test "Builtin: cd ~ (HOME shortcut) → then /bin/pwd" \
    "cd ~\n/bin/pwd"

#
# SECTION 3: Environment management (7 tests)
#
echo
echo "🔹 Section 3: Environment management (7 tests)"
echo "==========================================================================="

run_test "env → should display all key=value pairs" \
    "env"

run_test "setenv FOO bar → then env (should show FOO=bar)" \
    "setenv FOO bar\nenv"

run_test "echo \$FOO → should display 'bar'" \
    "setenv FOO bar\necho \$FOO"

run_test "/usr/bin/env → verify external 'env' inherits FOO" \
    "setenv FOO bar\n/usr/bin/env"

run_test "unsetenv FOO → then env (FOO should be gone)" \
    "setenv FOO bar\nunsetenv FOO\nenv"

run_test "unsetenv FOO again → env (still no FOO)" \
    "unsetenv FOO\nenv"

run_test "/usr/bin/env again → should not show FOO" \
    "/usr/bin/env"

#
# SECTION 4: PATH management (6 tests)
#
echo
echo "🔹 Section 4: PATH management (6 tests)"
echo "==========================================================================="

run_test "unsetenv PATH; setenv PATH \"/bin:/usr/bin\" → then ls" \
    "unsetenv PATH\nsetenv PATH /bin:/usr/bin\nls"

run_test "Run emacs (check if /usr/bin/emacs works)" \
    "emacs"

run_test "unsetenv PATH → then ls (should fail)" \
    "unsetenv PATH\nls"

run_test "unsetenv PATH → then emacs (should fail)" \
    "unsetenv PATH\nemacs"

run_test "/bin/ls → should succeed regardless of PATH" \
    "/bin/ls"

run_test "/usr/bin/emacs → absolute path (should succeed if installed)" \
    "/usr/bin/emacs"

#
# SECTION 5: Command-line management (4 tests)
#
echo
echo "🔹 Section 5: Command-line management (4 tests)"
echo "==========================================================================="

run_test "Empty command (just Enter) → no action, prompt returns" \
    ""

run_test "Single space only → no action, prompt returns" \
    " "

run_test "Spaces + tabs only → no action, prompt returns" \
    "   \t\t   "

run_test "Complex spacing:    /bin/ls   -l  -A   → should still work" \
    "   /bin/ls    -l   -A   "

#
# Finish
#
echo
echo "🎉 All automated (mandatory) tests completed successfully! No memory leaks found."
echo "   If any test had leaks, the script would have stopped already."
echo "   Full details are in the 'results' folder: $RESULT_DIR"
echo "==========================================================================="
exit 0

