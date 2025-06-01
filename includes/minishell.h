/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:00:52 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/30 18:28:47 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/includes/libft.h"
# include "colors.h"
# include "error.h"
# include "exec.h"
# include "heredoc.h"
# include <signal.h>
# include <curses.h>
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
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
# define DEBUG 0

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

	int							heredoc_counter;
	t_hd_temp_file				*active_heredocs;

	t_token						*token_list;
}								t_context;

extern volatile sig_atomic_t	g_signal;

/*-------------------parser functions--------------------------*/

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

/** lexer, parser utils and functions */
t_exec							*parser(t_token *token_list, t_context *ctx);
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
void							free_structs(char *errmsg, t_token *ptr_tkn,
									t_exec *ptr_exec);
t_args							*create_arg_node(char *value, t_context *ctx);
t_args							*append_arg(t_args **args_list_head,
									char *value, t_context *ctx);
int								process_redir_token(t_exec *cmd_node,
									t_token **curr_token_ptr, t_context *ctx);
int								append_eof_token(t_token **list_head,
									t_token **list_tail, t_context *ctx);

/** Other functions */
int								run_minishell(t_context *ctx);
int								cleanup_after_sigint(char *cmd,
									t_token **token_list);
int								cleanup_failed_exec(char *cmd,
									t_token **token_list);
int								check_state(int argc, char *argv[]);
char							*read_cmd(t_context *ctx);
int								clear_term(t_context *ctx);
void							set_exit_code(t_context *ctx, int exit_code,
									char *errmsg);
void							cleanup_tcontext(t_context *ctx);
void							cleanup_resources(char *cmd,
									t_token *token_list, t_context *ctx);
int								setup_signal_handlers(t_context *ctx);

/*-------------------heredoc functions--------------------------*/

char							*handle_heredoc(t_token **curr_token_ptr,
									t_context *ctx);
int								read_heredoc_input(t_heredoc_data *hdata,
									t_context *ctx);
int								heredoc_delim_validation(t_context *ctx,
									t_token *delim_token);
void							init_heredoc_struct(t_heredoc_data *hdata);
int								hd_tempfile(t_context *ctx, int count,
									char **path_out);
void							cleanup_hd_resources(t_heredoc_data *hdata);
void							add_active_heredoc(t_context *ctx,
									const char *filepath);
void							remove_and_unlink_active_heredoc(t_context *ctx,
									const char *filepath);
void							cleanup_all_active_heredocs(t_context *ctx);
int								handle_eof_case(t_heredoc_data *hdata);

/** ENVP functions */
void							free_env_list(t_env *env_list_head);
t_env							*populate_env_list(char **original_envp,
									t_context *ctx);

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
void							print_signal_msg(int status,
									int *message_printed);
void							setup_signal_parent(void);
void							setup_signal_child(void);

#endif