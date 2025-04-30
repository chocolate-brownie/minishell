# Lexical Analysis (Lexing or Tokenization)

Let's dive into **Lexical Analysis**, often called **lexing** or **tokenization**. This is the first step in understanding the user's command line input.

**The Goal:**

Imagine the raw command string like `cat file.txt | grep "search pattern" > output`. It's just a sequence of characters. The goal of the lexer is to chop this string into a sequence of meaningful pieces, called **tokens**, based on predefined rules. It's like breaking an English sentence into words and punctuation marks.

The output of the lexer for the example above would be a list of tokens like:

1.  `TOKEN_WORD` with value `"cat"`
2.  `TOKEN_WORD` with value `"file.txt"`
3.  `TOKEN_PIPE` with value `"|"`
4.  `TOKEN_WORD` with value `"\"search pattern\""` (Note: quotes are often kept initially)
5.  `TOKEN_REDIR_OUT` with value `">"`
6.  `TOKEN_WORD` with value `"output"`
7.  `TOKEN_EOF` (signaling the end)

**Core Concepts & How to Implement `lexer(const char *input)`:**

1.  **Scanning:** You'll process the `input` string sequentially, usually using an index or a pointer (`i` or `char *current`).

2.  **State:** While simple lexers can be stateless, it often helps to think about the "state" you're in. Are you inside single quotes? Double quotes? Or just reading a regular word?

3.  **Whitespace Handling:** Whitespace (spaces, tabs) typically acts as a delimiter *between* tokens. The lexer's job is usually to consume and *skip* leading whitespace before looking for the next token. Whitespace *inside* quotes, however, becomes part of the `TOKEN_WORD`.

4.  **Operator Recognition:** These are the special characters/sequences defined in your `t_token_type` enum:
    * `|`: Simple, one character. Create `TOKEN_PIPE`.
    * `<`: Simple, one character. Create `TOKEN_REDIR_IN`.
    * `>`: Needs lookahead. Check if the *next* character is also `>`.
        * If `>>`: Create `TOKEN_REDIR_APPEND`. Advance pointer by 2.
        * If just `>`: Create `TOKEN_REDIR_OUT`. Advance pointer by 1.
    * `<<`: Needs lookahead (similar to `>`). Check if the *next* character is `<`.
        * If `<<`: Create `TOKEN_REDIR_HEREDOC`. Advance pointer by 2.
        * If just `<`: Create `TOKEN_REDIR_IN`. Advance pointer by 1. (Make sure you handle `<` before `<<` or vice-versa consistently).

5.  **Quote Handling (`'` and `"`):** This is often the trickiest part.
    * When you encounter an opening quote (`'` or `"`), you enter a "quoted state".
    * You then read *all* characters, including whitespace and special operators (which lose their special meaning inside quotes, except sometimes `$` inside `"`), until you find the *matching* closing quote.
    * The entire sequence, *including the quotes themselves* for now, forms the `value` of a single `TOKEN_WORD`.
    * **Error Handling:** If you reach the end of the input string before finding the matching closing quote, it's a syntax error (unclosed quote). Your lexer should report this, likely by returning `NULL` and potentially printing an error message.

6.  **Word Recognition (`TOKEN_WORD`):** If the current character isn't whitespace, an operator, or a quote, it must be the start of a regular word (command, argument, filename).
    * Read characters sequentially until you hit:
        * Whitespace
        * An operator (`|`, `<`, `>`)
        * A quote (`'` or `"`)
        * The end of the input string (`\0`)
    * The sequence of characters read forms the `value` of the `TOKEN_WORD`.

7.  **Building the Token List:**
    * As you identify each token, you need to:
        * Allocate memory for a `t_token` struct (`malloc`). Handle allocation failures.
        * Extract the relevant substring from the input to be the token's `value` (using something like `ft_substr` from your `libft`). Allocate memory for this string. Handle allocation failures.
        * Set the token's `type`.
        * Append the new token to the end of your linked list of tokens. Keep track of the head and tail of the list.

8.  **End of Input (`TOKEN_EOF`):** When your scanning pointer reaches the null terminator (`\0`) of the input string, create and append the final `TOKEN_EOF` token.

**Simplified Algorithm Outline:**

Alright, let's dive into **Lexical Analysis**, often called **lexing** or **tokenization**. This is the first step in understanding the user's command line input.

**The Goal:**

Imagine the raw command string like `cat file.txt | grep "search pattern" > output`. It's just a sequence of characters. The goal of the lexer is to chop this string into a sequence of meaningful pieces, called **tokens**, based on predefined rules. It's like breaking an English sentence into words and punctuation marks.

The output of the lexer for the example above would be a list of tokens like:

1.  `TOKEN_WORD` with value `"cat"`
2.  `TOKEN_WORD` with value `"file.txt"`
3.  `TOKEN_PIPE` with value `"|"`
4.  `TOKEN_WORD` with value `"grep"`
5.  `TOKEN_WORD` with value `"\"search pattern\""` (Note: quotes are often kept initially)
6.  `TOKEN_REDIR_OUT` with value `">"`
7.  `TOKEN_WORD` with value `"output"`
8.  `TOKEN_EOF` (signaling the end)

**Core Concepts & How to Implement `lexer(const char *input)`:**

1.  **Scanning:** You'll process the `input` string sequentially, usually using an index or a pointer (`i` or `char *current`).

2.  **State:** While simple lexers can be stateless, it often helps to think about the "state" you're in. Are you inside single quotes? Double quotes? Or just reading a regular word?

3.  **Whitespace Handling:** Whitespace (spaces, tabs) typically acts as a delimiter *between* tokens. The lexer's job is usually to consume and *skip* leading whitespace before looking for the next token. Whitespace *inside* quotes, however, becomes part of the `TOKEN_WORD`.

4.  **Operator Recognition:** These are the special characters/sequences defined in your `t_token_type` enum:
    *   `|`: Simple, one character. Create `TOKEN_PIPE`.
    *   `<`: Simple, one character. Create `TOKEN_REDIR_IN`.
    *   `>`: Needs lookahead. Check if the *next* character is also `>`.
        *   If `>>`: Create `TOKEN_REDIR_APPEND`. Advance pointer by 2.
        *   If just `>`: Create `TOKEN_REDIR_OUT`. Advance pointer by 1.
    *   `<<`: Needs lookahead (similar to `>`). Check if the *next* character is `<`.
        *   If `<<`: Create `TOKEN_REDIR_HEREDOC`. Advance pointer by 2.
        *   If just `<`: Create `TOKEN_REDIR_IN`. Advance pointer by 1. (Make sure you handle `<` before `<<` or vice-versa consistently).

5.  **Quote Handling (`'` and `"`):** This is often the trickiest part.
    *   When you encounter an opening quote (`'` or `"`), you enter a "quoted state".
    *   You then read *all* characters, including whitespace and special operators (which lose their special meaning inside quotes, except sometimes `$` inside `"`), until you find the *matching* closing quote.
    *   The entire sequence, *including the quotes themselves* for now, forms the `value` of a single `TOKEN_WORD`.
    *   **Error Handling:** If you reach the end of the input string before finding the matching closing quote, it's a syntax error (unclosed quote). Your lexer should report this, likely by returning `NULL` and potentially printing an error message.

6.  **Word Recognition (`TOKEN_WORD`):** If the current character isn't whitespace, an operator, or a quote, it must be the start of a regular word (command, argument, filename).
    *   Read characters sequentially until you hit:
        *   Whitespace
        *   An operator (`|`, `<`, `>`)
        *   A quote (`'` or `"`)
        *   The end of the input string (`\0`)
    *   The sequence of characters read forms the `value` of the `TOKEN_WORD`.

7.  **Building the Token List:**
    *   As you identify each token, you need to:
        *   Allocate memory for a `t_token` struct (`malloc`). Handle allocation failures.
        *   Extract the relevant substring from the input to be the token's `value` (using something like `ft_substr` from your `libft`). Allocate memory for this string. Handle allocation failures.
        *   Set the token's `type`.
        *   Append the new token to the end of your linked list of tokens. Keep track of the head and tail of the list.

8.  **End of Input (`TOKEN_EOF`):** When your scanning pointer reaches the null terminator (`\0`) of the input string, create and append the final `TOKEN_EOF` token.

**Simplified Algorithm Outline:**

```
function lexer(input_string):
  token_list_head = NULL
  token_list_tail = NULL
  current_pos = 0

  while input_string[current_pos] != '\0':
    // 1. Skip Whitespace
    while input_string[current_pos] is whitespace:
      current_pos++

    // Check for EOF after skipping whitespace
    if input_string[current_pos] == '\0':
      break

    start_pos = current_pos
    token_type = determine_token_type(input_string, &current_pos) // This function handles operators, quotes, words

    if token_type is ERROR (e.g., unclosed quote):
      free token_list_head // Cleanup partially built list
      return NULL

    // Extract value string from input_string[start_pos] up to current_pos
    token_value = substring(input_string, start_pos, current_pos - start_pos)
    if allocation failed for token_value:
        free token_list_head
        return NULL

    // Create new token node
    new_token = create_token(token_value, token_type)
    if allocation failed for new_token:
        free token_value
        free token_list_head
        return NULL

    // Add token to list
    append_token(&token_list_head, &token_list_tail, new_token)

  // Add EOF token
  eof_token = create_token(NULL, TOKEN_EOF) // Value can be NULL for EOF
  if eof_token:
      append_token(&token_list_head, &token_list_tail, eof_token)
  else:
      // Handle allocation failure for EOF token
      free token_list_head
      return NULL


  return token_list_head

// Helper functions needed:
// determine_token_type: Checks for operators, quotes, words, advances current_pos
// create_token: Mallocs and initializes a t_token
// append_token: Adds a token to the linked list
// substring: Extracts a portion of the string (like ft_substr)
```
