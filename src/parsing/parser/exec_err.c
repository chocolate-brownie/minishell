/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:07:06 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/30 16:46:51 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	token_failure(t_exec *new_node, t_context *ctx)
{
	if (!new_node)
		return ;
	free_single_exec_node_content(new_node);
	free(new_node);
	if (ctx && g_signal == SIGINT)
		return ;
	if (ctx && ctx->last_exit_code == 0)
		set_exit_code(ctx, ERR_SYNTAX, NULL);
}

void	unexpected_token(t_exec *new_node, t_token *curr_tok, t_context *ctx)
{
	if (!ctx)
		return ;
	if (ctx->last_exit_code == 0)
	{
		if (curr_tok)
			set_exit_code(ctx, ERR_SYNTAX, curr_tok->value);
		else
			set_exit_code(ctx, ERR_SYNTAX, "|");
	}
	token_failure(new_node, ctx);
}

t_redir_type	get_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_INPUT);
	if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUTPUT);
	if (type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	if (type == TOKEN_REDIR_HEREDOC)
		return (REDIR_HEREDOC);
	return (-1);
}
