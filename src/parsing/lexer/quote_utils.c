/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:00:00 by ai-assistan       #+#    #+#             */
/*   Updated: 2025/06/01 21:06:05 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*
** Appends a single character `c` to string `str`.
** Uses `append_extracted` for the core logic.
** Frees original `str` and returns new string or NULL on error.
*/
static char	*append_char(char *str, char c, t_context *ctx)
{
	char	add[2];
	char	*new_str;

	add[0] = c;
	add[1] = '\0';
	new_str = append_extracted(str, ft_strdup(add), ctx);
	return (new_str);
}

/*
** Finds the position immediately after the closing quote for a quote starting
** at `start_pos` in `cmd` string.
** Returns QUOTE_ERROR for invalid args, QUOTE_UNCLOSED if no closing quote
** is found, or the index after the closing quote.
*/
int	find_closing_quote(const char *cmd, int start_pos)
{
	char	quote_char;
	int		i;

	if (!cmd || start_pos < 0)
		return (QUOTE_ERROR);
	quote_char = cmd[start_pos];
	if (quote_char != '\'' && quote_char != '"')
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

/*
** Helper function for get_quoted_val to process characters within quotes.
** Handles '$?' expansion and appends characters to accumulated_value.
** Updates 'i' to the current processing position.
** Returns 0 to continue to next char, 1 on success, -1 on error.
*/
int	process_char_in_quoted_val(t_char_iter_params *params,
								char **accumulated_value,
								t_context *ctx)
{
	char	*temp_val;

	temp_val = *accumulated_value;
	if ((params->q_char == '"' && params->cmd_str[*(params->idx_ptr)] == '$')
		&& (params->cmd_str[*(params->idx_ptr) + 1] == '?'))
	{
		temp_val = expand_dollar_question(temp_val, params->idx_ptr, ctx);
		if (!temp_val)
			return (-1);
		*accumulated_value = temp_val;
		return (0);
	}
	else
	{
		temp_val = append_char(temp_val, params->cmd_str[*(params->idx_ptr)],
				ctx);
		if (!temp_val)
			return (-1);
		(*(params->idx_ptr))++;
	}
	*accumulated_value = temp_val;
	return (1);
}

/*
** Helper function for get_unquoted_val to process characters.
** Handles '$?' expansion and appends characters to accumulated_value.
** Updates 'i' to the current processing position.
** Returns 0 to continue, 1 on success, -1 on error.
*/
int	process_char_in_unquoted_val(const char *command, int *i,
		char **accumulated_value, t_context *ctx)
{
	char	*temp_val;

	temp_val = *accumulated_value;
	if (command[*i] == '$' && command[(*i) + 1] == '?')
	{
		temp_val = expand_dollar_question(temp_val, i, ctx);
		if (!temp_val)
			return (-1);
		*accumulated_value = temp_val;
		return (0);
	}
	else
	{
		temp_val = append_char(temp_val, command[*i], ctx);
		if (!temp_val)
			return (-1);
		(*i)++;
	}
	*accumulated_value = temp_val;
	return (1);
}
