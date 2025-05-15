/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_tkn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:17:14 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/10 23:51:54 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static t_token	*handle_less_than(const char *cmd, int *i, t_context *ctx)
{
	t_token	*token;

	if (!cmd || !i || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), NULL);
	if (cmd[*i + 1] == '<')
	{
		*i += 2;
		token = create_token("<<", TOKEN_REDIR_HEREDOC);
	}
	else
	{
		(*i)++;
		token = create_token("<", TOKEN_REDIR_IN);
	}
	if (!token)
		return (set_exit_code(ctx, ERR_MALLOC, NULL), NULL);
	return (token);
}

static t_token	*handle_greater_than(const char *cmd, int *i, t_context *ctx)
{
	t_token	*token;

	if (!cmd || !i || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), NULL);
	if (cmd[*i + 1] == '>')
	{
		*i += 2;
		token = create_token(">>", TOKEN_REDIR_APPEND);
	}
	else
	{
		(*i)++;
		token = create_token(">", TOKEN_REDIR_OUT);
	}
	if (!token)
		return (set_exit_code(ctx, ERR_MALLOC, NULL), NULL);
	return (token);
}

t_token	*get_next_token(const char *cmd, int *i, t_context *ctx)
{
	t_token	*token;

	if (!cmd || !i || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), NULL);
	while (cmd[*i] != '\0' && ft_isspace(cmd[*i]))
		(*i)++;
	if (cmd[*i] == '\0')
		return (NULL);
	if (cmd[*i] == '|')
	{
		(*i)++;
		token = create_token("|", TOKEN_PIPE);
	}
	else if (cmd[*i] == '<')
		token = handle_less_than(cmd, i, ctx);
	else if (cmd[*i] == '>')
		token = handle_greater_than(cmd, i, ctx);
	else
		token = handle_word(cmd, i, ctx);
	if (!token)
		return (set_exit_code(ctx, ERR_MALLOC, NULL), NULL);
	return (token);
}
