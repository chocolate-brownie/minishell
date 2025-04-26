/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:00:52 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/26 18:30:11 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include "colors.h"
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
}					t_token_type;
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
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;

}					t_token;

/**
This is the main system where we store all the importnat
information that different parts of the shell need to access or modify
throughout its lifecycle

this structure will used by both for pasrsing and execution

Many different functions need the same data:
* Your parser needs the environment variables to expand $VAR and the last exit
	code to expand $?.
* Your executor needs the environment variables to pass to execve, the list
	of parsed commands (t_exec*) to execute, and it needs to set the exit code
	when commands finish
* Built-in commands like cd, export, unset need to read and modify the
	environment variables. exit needs to set the exit code.
* The main loop needs to know the exit code to potentially terminate and needs
	to manage the overall state.
* Passing individual pointers (like envp, exit_code, parsed_commands, etc.)
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

}					t_context;
*/

typedef struct s_env
{
	char			*id;
	char			*value;
	char			*raw;
	struct s_env	*next;
}					t_env;

typedef struct s_context
{
	t_env			*envp;
	int				last_exit_code;
	int				should_exit;

	t_exec			*command_list;

	pid_t			*pids;
	int				pid_count;
	int				stdin_backup;
	int				stdout_backup;

}					t_context;

/** Main lexical analysis function */
t_token				*lexer(const char *cmd);

/** Tokenization functions & tools */
t_token				*get_next_token(const char *cmd, int *i);
t_token				*create_token(char *value, t_token_type type);
void				free_token_list(t_token *token_list);
void				append_token(t_token **head, t_token **tail,
						t_token *token);

/** other functions */
int					check_state(int argc, char *argv[]);
char				*read_cmd(void);
int					clear_term(void);
void				perr(char *err_msg);

/** debugging functions */
const char			*token_type_to_string(t_token_type type);
void				print_tokens(char *cmd, t_token *list_head);

#endif
