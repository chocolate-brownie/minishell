/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:18:46 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/01 00:11:34 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/* 
TODO: 
When find_closing_quote fails, it just prints "Error: Unclosed quote\\n". 
While correct, shells often provide more context, like the line number or 
approximate location. This is an enhancement, not strictly necessary now, 
but something to keep in mind for later refinement.
*/

int	find_closing_quote(const char *cmd, int start_pos)
{
	char	quote_char;
	int		i;

	quote_char = cmd[start_pos];
	if (quote_char != '\'' && quote_char != '\"')
		return (-1);
	i = start_pos + 1;
	while (cmd[i] != '\0')
	{
		if (cmd[i] == quote_char)
			return (i + 1);
		i++;
	}
	write(2, "Error: Unclosed quote\n", 22);
	return (-1);
}

static char	*get_quoted_val(const char *command, int *position)
{
	int		start;
	int		end;
	int		length;
	char	*value;

	start = *position;
	end = find_closing_quote(command, start);
	if (end == -1)
		return (NULL);
	length = (end - start) - 2;
	if (length < 0)
		length = 0;
	value = ft_substr(command, start + 1, length);
	if (!value)
		return (perror("malloc failed in get_quoted_val"), NULL);
	*position = end;
	return (value);
}

static char	*get_unquoted_val(const char *command, int *position)
{
	int		start;
	int		i;
	int		length;
	char	*value;

	start = *position;
	i = start;
	while (command[i] && !is_delimiter(command[i]) && !is_quote(command[i]))
		i++;
	length = i - start;
	if (length <= 0)
		return (NULL);
	value = ft_substr(command, start, length);
	if (!value)
		return (perror("malloc failed in get_unquoted_val"), NULL);
	*position = i;
	return (value);
}

char	*handle_quotes(const char *cmd, int *index)
{
	int		curr_pos;
	char	*value;

	curr_pos = *index;
	if (is_quote(cmd[curr_pos]))
		value = get_quoted_val(cmd, &curr_pos);
	else
		value = get_unquoted_val(cmd, &curr_pos);
	if (!value)
		return (perror("malloc failed handle_quotes"), NULL);
	*index = curr_pos;
	return (value);
}

char	*append_extracted_value(char *accumulated_value, char *value)
{
	char	*temp;

	temp = ft_strjoin(accumulated_value, value);
	free(accumulated_value);
	free(value);
	if (!temp)
		return (perror("malloc failed append_extracted_value"), NULL);
	accumulated_value = temp;
	return (accumulated_value);
}
