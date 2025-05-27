/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:00:52 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/26 15:54:16 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include "colors.h"
# include "error.h"
# include "exec.h"
# include <curses.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

# define PROMPT "minishell → "
# define DEBUG 1

# define ERR_SUCCESS 0
# define ERR_MALLOC 1
# define ERR_SYNTAX 2
# define ERR_INVALID_INPUT 3
# define ERR_CMD_NOT_FOUND 127
# define ERR_PERMISSION_DENIED 126
# define ERR_PIPE 1
# define ERR_UNCLOSED_QUOTE 2

# define QUOTE_ERROR -1
# define QUOTE_UNCLOSED -2

extern volatile sig_atomic_t	g_signal_received;

/**
typedef enum e_token_type
{
	TOKEN_WORD, - A sequence of characters (command, argument, filename, etc.)
	TOKEN_PIPE, - |
	TOKEN_REDIR_IN, - <
	TOKEN_REDIR_OUT, - >
	TOKEN_REDIR_APPEND,	- >>
	TOKEN_REDIR_HEREDOC,- <<
	TOKEN_EOF			- End of input marker
}								t_token_type;
*/
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_EOF
}								t_token_type;

typedef struct s_token
{
	char						*value;
	t_token_type				type;
	struct s_token				*next;

}								t_token;

/**
This is the main system where we store all the importnat
information that different parts of the shell need to access or modify
throughout its lifecycle

this structure will used by both for pasrsing and execution

Many different functions need the same data:
* Your parser needs the environment variables to expand $VAR and the last exit
	code to expand $?.
* Your executor needs the environment variables to pass to execve, the list
	of parsed commands (t_exec*e to execute, and it needs to set the exit code
	when commands finish
* Built-in commands like cd, export, unset need to read and modify the
	environment variables. exit needs to set the exit code.
* The main loop needs to know the exit code to potentially terminate and needs
	to manage the overall state.
* Passing individual pointers (like envp, exit_code, parsed_commands, etc.e
	to every single function that might need them would be incredibly messy and
	unmanageable.

WHAT SHOULD GO INSIDE THIS STRUCTURE?

1. Environment Variables: A pointer to the head of your linked list
	representing the environment variables (e.g.,t_env *envp;). This is probably
	the most critical piece of shared data. Your parser reads it, builtins
	read/write it, and the executor reads it.

2. Last Exit Status: An integer (or unsigned char as in the reference) to store
	the exit code of the most recently executed command
	(e.g., int last_exit_code;). Your parser reads this for $? expansion, and
	the executor/builtins update it

3. Parsed Command List: While the parser creates the t_exec list and the
	executor consumes it, the main loop often holds a pointer to this list
	temporarily. The context struct could hold this pointer (like t_exec *exec;
	in the reference code), or it could be passed separately between the parser
	and executor via the main loop. Discuss this with your partner – keeping it
	in the context is common

4. Shell State Flags: Maybe boolean flags like int should_exit; that the exit
	builtin can set to signal the main loop to terminate.

5. (For Executor) Process IDs: Your partner might want fields to manage child
	process PIDs for waiting (like pid_t *pids; and int pid_count;
	in the reference).

6. (For Executor) Default File Descriptors: To handle redirections correctly,
	the executor often needs to save the original STDIN_FILENO and
	STDOUT_FILENO. The context is a good place for this (like int def_in;
	int def_out; in the reference).

typedef struct s_context {
	--- Data ALL parts might need ---
	t_env *envp;             -- Pointer to the Environment List
	int   last_exit_code;    -- Status of the last executed command ($?)
	int   should_exit;       -- Flag to signal the main loop to terminate

	--- Data primarily for PARSER -> EXECUTOR flow ---
	t_exec *command_list;
	-- Head of the parsed command pipeline (created by parser, used by executor)

	--- Data primarily for EXECUTOR internal use ---
	pid_t *pids;             -- Array to store child process IDs
	int   pid_count;         -- Number of PIDs currently stored
	int   stdin_backup;      -- Backup of original standard input FD (usually 0)
	int   stdout_backup;
	-- Backup of original standard output FD (usually 1)

}								t_context;
*/

typedef struct s_env
{
	char						*id;
	char						*value;
	char						*raw;
	struct s_env				*next;
}								t_env;

typedef struct s_context
{
	t_env						*envp;
	int							last_exit_code;
	int							should_exit;

	t_exec						*command_list;

	pid_t						*pids;
	int							pid_count;
	int							stdin_backup;
	int							stdout_backup;

}								t_context;

/** Main lexical analysis function */
t_token							*lexer(const char *cmd, t_context *ctx);

/** Tokenization functions & tools */
int								is_quote(char c);
int								is_delimiter(char c);
t_token							*get_next_token(const char *cmd, int *i,
									t_context *ctx);
t_token							*create_token(char *value, t_token_type type);
void							free_token_list(t_token *token_list);
void							append_token(t_token **head, t_token **tail,
									t_token *token);

/** Tokenization of words functions & tools */
int								find_closing_quote(const char *cmd,
									int start_pos);
char							*append_extracted(char *accumulated_value,
									char *value, t_context *ctx);
char							*handle_quotes(const char *cmd, int *index,
									t_context *ctx);
t_token							*handle_word(const char *cmd, int *i,
									t_context *ctx);

/** Parsing */
t_exec							*parser(t_token *token_list, t_context *ctx);

/** Parsing utils */
int								validate_init_tokens(t_token *token_list,
									t_context *ctx);
void							link_nodes(t_exec **head, t_exec **tail,
									t_exec *new_node);
int								check_next_tok(t_token **curr_tok,
									t_context *ctx);
void							invalseg_after_pipe(t_exec **headptr,
									t_exec **newptr, t_token **startptr,
									t_context *ctxptr);
void							free_exec_list(t_exec *head);
t_redir_type					get_redir_type(t_token_type type);
t_exec							*create_exec_node(t_token **token_ptr,
									t_context *ctx);
void							free_single_exec_node_content(t_exec *node);
void							token_failure(t_exec *new_node, t_context *ctx);
void							unexpected_token(t_exec *new_node,
									t_token *curr_tok, t_context *ctx);

t_redirs						*append_redir(t_redirs **redir_list_head,
									t_redir_type type, char *path,
									t_context *ctx);
int								process_word_token(t_exec *exec_node,
									t_token **curr, t_context *ctx);
int								process_redir_token(t_exec *exec_node,
									t_token **curr, t_redir_type redir_type,
									t_context *ctx);
void							free_structs(char *errmsg, t_token *ptr_tkn,
									t_exec *ptr_exec);

/** Other functions */
int								run_minishell(t_context *ctx);
int								check_state(int argc, char *argv[]);
char							*read_cmd(t_context *ctx);
int								clear_term(t_context *ctx);
void							set_exit_code(t_context *ctx, int exit_code,
									char *errmsg);
void							cleanup_tcontext(t_context *ctx);
void							cleanup_resources(char *cmd,
									t_token *token_list, t_context *ctx);

/** Debugging functions */
void							print_tokens(char *cmd, t_token *list_head);
void							print_exec_list(t_exec *exec_list_head);

/*-------------------execution functions--------------------------*/

/*env*/
t_env							*init_env(char **envp);
void							free_env(t_env *env);
t_env							*env_new_node(char *id, char *value, char *raw);
int								update_env_var(t_env *env, char *id,
									char *new_value);
char							*get_env_value(t_env *env, char *id);

/*exec utils*/
int								handle_redir(t_exec *cmd, t_context *ctx);
int								restore_stdio(t_context *ctx);
char							**args_to_array(t_exec *cmd,
									int include_cmd_name);
char							*get_cmd_path(char **envp, char *cmd);
pid_t							fork_and_execute(t_context *ctx, t_exec *cmd,
									int pipefd[2], int prev_read_end);
int								is_builtin(t_exec *cmd);
int								execute_builtin(t_exec *cmd, t_context *ctx,
									t_env *env);
char							**env_to_envp(t_env *env);
int								execute_pipeline(t_context *ctx);
int								create_pipe_if_needed(t_exec *cmd,
									int pipefd[2]);

/*free*/
void							free_tab(char **array);
void							free_all(t_context *ctx);
void							free_child(t_resources *res, t_context *ctx);

/*builtins*/
int								ft_cd(t_exec *cmd, t_env *env);
int								ft_echo(t_exec *cmd);
int								ft_pwd(void);
int								ft_env(t_env *env);
int								ft_export(t_env *env, t_exec *cmd);
int								ft_unset(t_env **env, t_exec *cmd);
void							ft_exit(t_exec *cmd, t_context *ctx);

/*signals*/
void							print_signal_msg(int status);
void							setup_signal_parent(void);
void							setup_signal_child(void);

#endif
