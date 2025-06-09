/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 18:58:00 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/09 22:48:17 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_redir_type	get_exec_redir_type_from_token(t_token_type token_type)
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

int	handle_invalid_token_syntax(t_token *file_token, t_context *ctx)
{
	if (file_token == NULL || file_token->type == TOKEN_EOF)
	{
		set_exit_code(ctx, ERR_SYNTAX, "newline");
		return (1);
	}
	if (file_token->type == TOKEN_PIPE || file_token->type != TOKEN_WORD)
	{
		set_exit_code(ctx, ERR_SYNTAX, file_token->value);
		return (1);
	}
	return (0);
}
