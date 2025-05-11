/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:18:46 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/10 23:59:12 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	find_closing_quote(const char *cmd, int start_pos)
{
	char	quote_char;
	int		i;

	if (!cmd || start_pos < 0)
		return (QUOTE_ERROR);
	quote_char = cmd[start_pos];
	if (quote_char != '\'' && quote_char != '\"')
		return (QUOTE_ERROR);
	i = start_pos + 1;
	while (cmd[i] != '\0')
	{
		if (cmd[i] == quote_char)
			return (i + 1);
		i++;
	}
	return (QUOTE_UNCLOSED);
}

static char	*get_quoted_val(const char *command, int *position, t_context *ctx)
{
	int		start;
	int		end;
	int		length;
	char	*value;

	if (!command || !position || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), NULL);
	start = *position;
	end = find_closing_quote(command, start);
	if (end == QUOTE_UNCLOSED)
		return (set_exit_code(ctx, ERR_SYNTAX,
				"minishell: syntax error: unclosed quote\n"), NULL);
	if (end == QUOTE_ERROR)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), NULL);
	length = (end - start) - 2;
	if (length < 0)
		length = 0;
	value = ft_substr(command, start + 1, length);
	if (!value)
		return (set_exit_code(ctx, ERR_MALLOC,
				"minishell: memory allocation failed\n"), NULL);
	*position = end;
	return (value);
}

static char	*get_unquoted_val(const char *command, int *position,
		t_context *ctx)
{
	int		start;
	int		i;
	int		length;
	char	*value;

	if (!command || !position || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), NULL);
	start = *position;
	i = start;
	while (command[i] && !is_delimiter(command[i]) && !is_quote(command[i]))
		i++;
	length = i - start;
	if (length <= 0)
		return (NULL);
	value = ft_substr(command, start, length);
	if (!value)
		return (set_exit_code(ctx, ERR_MALLOC, NULL), NULL);
	*position = i;
	return (value);
}

char	*handle_quotes(const char *cmd, int *index, t_context *ctx)
{
	int		curr_pos;
	char	*value;

	if (!cmd || !index || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), NULL);
	curr_pos = *index;
	if (is_quote(cmd[curr_pos]))
		value = get_quoted_val(cmd, &curr_pos, ctx);
	else
		value = get_unquoted_val(cmd, &curr_pos, ctx);
	if (!value)
		return (NULL);
	*index = curr_pos;
	return (value);
}

char	*append_extracted(char *accumulated_value, char *value, t_context *ctx)
{
	char	*temp;

	if (!value)
		return (accumulated_value);
	if (!accumulated_value)
		return (value);
	temp = ft_strjoin(accumulated_value, value);
	free(accumulated_value);
	free(value);
	if (!temp)
		return (set_exit_code(ctx, ERR_MALLOC, NULL), NULL);
	return (temp);
}
