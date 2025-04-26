/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_tkn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:17:14 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/26 17:26:03 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	is_delimiter(char c)
{
	return (ft_isspace(c) || c == '|' || c == '<' || c == '>' || c == '\0');
}

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

/**
TODO: handle the '' / "" handling logic inside the while loop
* @param handle_word function */
static t_token	*handle_word(const char *cmd, int *i)
{
	int		start;
	int		len;
	char	*word;
	t_token	*token;

	start = *i;
	while (cmd[*i] && !is_delimiter(cmd[*i]))
	{
		(*i)++;
	}
	len = *i - start;
	if (len == 0)
		return (NULL);
	word = ft_substr(cmd, start, len);
	if (!word)
	{
		perror("malloc failed for word value");
		return (NULL);
	}
	token = create_token(word, TOKEN_WORD);
	free(word);
	return (token);
}

/* TODO: in this function we need to implement an else if condition to check
for the single quote and double quote situation */

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
