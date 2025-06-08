/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_expansion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:30:00 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/08 16:53:48 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*handle_dollar_question(char *result, int *i, t_context *ctx)
{
	char	*temp;

	temp = ft_itoa(ctx->last_exit_code);
	if (!temp)
		return (free(result), NULL);
	result = append_extracted(result, temp, ctx);
	if (!result)
		return (NULL);
	*i += 2;
	return (result);
}

static char	*handle_variable_expansion(char *result, char *line,
										int *i, t_context *ctx)
{
	char	*var_name;
	char	*var_value;
	int		start;

	(*i)++;
	start = *i;
	while (line[*i] && is_valid_var_char_subsequent(line[*i]))
		(*i)++;
	var_name = ft_substr(line, start, *i - start);
	if (!var_name)
		return (free(result), NULL);
	var_value = get_env_value(ctx->envp, var_name);
	free(var_name);
	if (var_value)
	{
		result = append_extracted(result, var_value, ctx);
		if (!result)
			return (NULL);
	}
	return (result);
}

static char	*process_dollar_expansion(char *result, char *line,
										int *i, t_context *ctx)
{
	if (line[*i + 1] == '?')
		return (handle_dollar_question(result, i, ctx));
	else if (is_valid_var_char_start(line[*i + 1]))
		return (handle_variable_expansion(result, line, i, ctx));
	else
	{
		result = append_char(result, line[*i], ctx);
		if (!result)
			return (NULL);
		(*i)++;
		return (result);
	}
}

static char	*process_character(char *result, char *line, int *i,
								t_context *ctx)
{
	while (line[*i])
	{
		if (line[*i] == '$')
		{
			result = process_dollar_expansion(result, line, i, ctx);
			if (!result)
				return (NULL);
			continue ;
		}
		result = append_char(result, line[*i], ctx);
		if (!result)
			return (NULL);
		(*i)++;
	}
	return (result);
}

char	*expand_heredoc_line(char *line, t_context *ctx)
{
	char	*result;
	int		i;

	if (!line)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	return (process_character(result, line, &i, ctx));
}
