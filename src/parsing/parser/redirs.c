/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:23:45 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/26 17:09:49 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static t_redir_type	get_exec_redir_type_from_token(t_token_type token_type)
{
	if (token_type == TOKEN_REDIR_IN)
		return (REDIR_INPUT);
	if (token_type == TOKEN_REDIR_OUT)
		return (REDIR_OUTPUT);
	if (token_type == TOKEN_REDIR_HEREDOC)
		return (REDIR_HEREDOC);
	if (token_type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	return ((t_redir_type)-1);
}

/** Handle all the other redirections except for the heredoc, heredoc should
have his own seperate folders and shit... */
static char	*handle_other_redirs(t_token **curr_token_ptr, t_context *ctx)
{
	t_token	*operator_token;
	t_token	*file_token;

	if (!curr_token_ptr || !*curr_token_ptr)
		return (NULL);
	operator_token = *curr_token_ptr;
	file_token = operator_token->next;
	if (file_token == NULL || file_token->type != TOKEN_WORD)
	{
		if (file_token && ctx)
			set_exit_code(ctx, ERR_SYNTAX, file_token->value);
		else if (ctx)
			set_exit_code(ctx, ERR_SYNTAX, "newline");
		return (NULL);
	}
	*curr_token_ptr = file_token->next;
	return (file_token->value);
}

/** Parses the redirection path/delimiter and determines its type.
Advances *curr_token_ptr past the operator and its argument. */
static char	*get_redir_path_and_set_type(t_token **curr_token_ptr,
											t_context *ctx,
											t_redir_type *redir_type_val_out)
{
	t_token	*operator_token;
	char	*path_arg_val;

	operator_token = *curr_token_ptr;
	*redir_type_val_out = get_exec_redir_type_from_token(operator_token->type);
	if (*redir_type_val_out == (t_redir_type)-1)
		return (set_exit_code(ctx, ERR_SYNTAX, ERMSG_REDIR_UNKNOWNTOK), NULL);
	if (*redir_type_val_out == REDIR_HEREDOC)
		path_arg_val = handle_heredoc(curr_token_ptr, ctx);
	else
		path_arg_val = handle_other_redirs(curr_token_ptr, ctx);
	return (path_arg_val);
}

/** Attempts to append the redirection to the command's list and handles
cleanup of the original path_arg (especially for heredocs). */
static int	do_append_and_cleanup(t_exec *cmd_node,
									t_redir_type exec_redir_type,
									char *path_arg,
									t_context *ctx)
{
	int	is_heredoc;

	is_heredoc = (exec_redir_type == REDIR_HEREDOC);
	if (append_redir(&(cmd_node->redirs), exec_redir_type, path_arg,
			ctx) == NULL && ctx->last_exit_code != 0)
	{
		if (is_heredoc)
		{
			unlink(path_arg);
			free(path_arg);
		}
		return (-1);
	}
	if (is_heredoc)
		free(path_arg);
	return (0);
}

/** Processes a redirection token. Parses redirection details, 
then appends and cleans up. This should be so fucking annoying*/
int	process_redir_token(t_exec *cmd_node, t_token **curr_token_ptr,
		t_context *ctx)
{
	char			*path_val;
	t_redir_type	type_val;

	if (!cmd_node || !curr_token_ptr || !*curr_token_ptr || !ctx)
	{
		if (ctx)
			set_exit_code(ctx, ERR_INVALID_INPUT, ERMSG_REDIR_ARGS);
		return (-1);
	}
	path_val = get_redir_path_and_set_type(curr_token_ptr, ctx, &type_val);
	if (path_val == NULL)
		return (-1);
	return (do_append_and_cleanup(cmd_node, type_val, path_val, ctx));
}
