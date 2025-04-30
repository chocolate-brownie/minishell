/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_tkn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:17:14 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/30 23:33:40 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static t_token	*handle_less_than(const char *cmd, int *i)
{
	if (cmd[*i + 1] == '<')
	{
		*i += 2;
		return (create_token("<<", TOKEN_REDIR_HEREDOC));
	}
	else
	{
		(*i)++;
		return (create_token("<", TOKEN_REDIR_IN));
	}
}

static t_token	*handle_greater_than(const char *cmd, int *i)
{
	if (cmd[*i + 1] == '>')
	{
		*i += 2;
		return (create_token(">>", TOKEN_REDIR_APPEND));
	}
	else
	{
		(*i)++;
		return (create_token(">", TOKEN_REDIR_OUT));
	}
}

t_token	*get_next_token(const char *cmd, int *i)
{
	while (cmd[*i] != '\0' && ft_isspace(cmd[*i]))
		(*i)++;
	if (cmd[*i] == '\0')
		return (NULL);
	if (cmd[*i] == '|')
	{
		(*i)++;
		return (create_token("|", TOKEN_PIPE));
	}
	else if (cmd[*i] == '<')
		return (handle_less_than(cmd, i));
	else if (cmd[*i] == '>')
		return (handle_greater_than(cmd, i));
	else
		return (handle_word(cmd, i));
}
