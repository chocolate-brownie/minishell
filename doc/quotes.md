## The Importance of Quote Handling in Shell Tokenization: Real Examples

Let's explore why correctly handling quotes is crucial during the tokenization phase of a shell, using `echo` command examples that you can run in bash or zsh. The `echo` command helps us see exactly how the shell interprets arguments.

### 1. Spaces - Unquoted vs. Quoted

* **Command:** `echo hello world`
    * **Lexer Goal:** `WORD("echo")`, `WORD("hello")`, `WORD("world")`
    * **Shell Behavior:** `echo` receives **two** separate arguments: "hello" and "world".
    * **Output:** `hello world`

* **Command:** `echo "hello world"`
    * **Lexer Goal:** `WORD("echo")`, `WORD("hello world")`
    * **Shell Behavior:** `echo` receives **one** argument: "hello world". The double quotes group the words.
    * **Output:** `hello world`

* **Command:** `echo 'hello world'`
    * **Lexer Goal:** `WORD("echo")`, `WORD("hello world")`
    * **Shell Behavior:** `echo` receives **one** argument: "hello world". Single quotes also group words.
    * **Output:** `hello world`

### 2. Special Characters & Operators

* **Command:** `echo *`
    * **Lexer Goal:** `WORD("echo")`, `WORD("*")`
    * **Shell Behavior:** The `*` is **not** treated literally. The shell expands it (pathname expansion) into a list of files in the current directory. `echo` receives multiple arguments (the filenames).
    * **Output:** (List of files in your current directory, e.g., `Makefile README.md src include libft`)

* **Command:** `echo "*"`
    * **Lexer Goal:** `WORD("echo")`, `WORD("*")`
    * **Shell Behavior:** The double quotes prevent pathname expansion. `echo` receives **one** literal argument: "*".
    * **Output:** `*`

* **Command:** `echo '*'`
    * **Lexer Goal:** `WORD("echo")`, `WORD("*")`
    * **Shell Behavior:** Single quotes also prevent pathname expansion. `echo` receives **one** literal argument: "*".
    * **Output:** `*`

* **Command:** `echo hello > file`
    * **Lexer Goal:** `WORD("echo")`, `WORD("hello")`, `REDIR_OUT(">")`, `WORD("file")`
    * **Shell Behavior:** Executes `echo` with argument "hello", redirecting its output to `file`.
    * **Output:** (Nothing printed to terminal, "hello" written to `file`)

* **Command:** `echo "hello > file"`
    * **Lexer Goal:** `WORD("echo")`, `WORD("hello > file")`
    * **Shell Behavior:** The `>` is inside quotes, so it's treated as a literal character. `echo` receives one argument: "hello > file".
    * **Output:** `hello > file`

### 3. Single vs. Double Quotes (Expansion)

* **Set a variable:** `MYVAR="hello"` (Type this in your terminal first)
* **Command:** `echo $MYVAR world`
    * **Lexer Goal (Simplified):** `WORD("echo")`, `WORD("$MYVAR")`, `WORD("world")` (Expansion happens *after* lexing)
    * **Shell Behavior:** The shell expands `$MYVAR` to "hello". `echo` receives "hello" and "world".
    * **Output:** `hello world`

* **Command:** `echo "$MYVAR world"`
    * **Lexer Goal:** `WORD("echo")`, `WORD("$MYVAR world")`
    * **Shell Behavior:** Double quotes **allow** variable expansion. `$MYVAR` becomes "hello". `echo` receives one argument: "hello world".
    * **Output:** `hello world`

* **Command:** `echo '$MYVAR world'`
    * **Lexer Goal:** `WORD("echo")`, `WORD("$MYVAR world")`
    * **Shell Behavior:** Single quotes **prevent** variable expansion. `echo` receives one literal argument: "$MYVAR world".
    * **Output:** `$MYVAR world`

### 4. Concatenation

* **Command:** `echo "hello"' world'`
    * **Lexer Goal:** `WORD("echo")`, `WORD("hello world")`
    * **Shell Behavior:** Adjacent quoted (or unquoted) parts are joined into a single word *before* the command gets it. `echo` receives one argument: "hello world".
    * **Output:** `hello world`

* **Command:** `echo hello" "world`
    * **Lexer Goal:** `WORD("echo")`, `WORD("hello world")`
    * **Shell Behavior:** Same as above. `echo` receives one argument: "hello world".
    * **Output:** `hello world`

These examples clearly illustrate that the lexer plays a vital role in correctly identifying word boundaries, especially when quotes are involved. This ensures that subsequent stages of the shell (like parsing and expansion) can interpret the user's command according to the defined shell rules. Your `handle_word` function is indeed a critical component where this interpretation of boundaries takes place.