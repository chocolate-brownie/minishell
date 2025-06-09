/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:23:45 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/09 19:01:57 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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
	if (handle_invalid_token_syntax(file_token, ctx, curr_token_ptr))
		return (NULL);
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
			ctx) == NULL)
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
