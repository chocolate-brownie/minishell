/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:00:00 by ai-assistan       #+#    #+#             */
/*   Updated: 2025/06/04 17:37:27 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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

static int	handle_general_dollar_or_literal_dq(t_char_iter_params *params,
												char **val_ptr,
												t_context *ctx,
												char dollar_char)
{
	char	*var_name;
	char	*var_value;

	if (is_valid_var_char_start(params->cmd_str[*(params->idx_ptr) + 1]))
	{
		(*(params->idx_ptr))++;
		var_name = extract_env_var_name(params->cmd_str, params->idx_ptr);
		if (var_name)
		{
			var_value = get_env_value(ctx->envp, var_name);
			if (var_value)
				*val_ptr = append_extracted(*val_ptr, var_value, ctx);
			free(var_name);
			if (!*val_ptr && var_value)
				return (-1);
			return (1);
		}
		(*(params->idx_ptr))--;
	}
	*val_ptr = append_char(*val_ptr, dollar_char, ctx);
	if (!*val_ptr)
		return (-1);
	(*(params->idx_ptr))++;
	return (1);
}

static int	handle_dollar_dq(t_char_iter_params *params,
							char **val_ptr,
							t_context *ctx)
{
	char	current_char_val;

	current_char_val = params->cmd_str[*(params->idx_ptr)];
	if (params->cmd_str[*(params->idx_ptr) + 1] == '?')
	{
		*val_ptr = expand_dollar_question(*val_ptr, params->idx_ptr, ctx);
		if (!*val_ptr)
			return (-1);
	}
	else
	{
		return (handle_general_dollar_or_literal_dq(params, val_ptr, ctx,
				current_char_val));
	}
	return (1);
}

static int	process_double_quote_char_val(t_char_iter_params *params,
											char **val_ptr,
											t_context *ctx)
{
	char	current_char_val;

	current_char_val = params->cmd_str[*(params->idx_ptr)];
	if (current_char_val == '\\')
		return (handle_backslash_dq(params, val_ptr, ctx));
	else if (current_char_val == '$')
		return (handle_dollar_dq(params, val_ptr, ctx));
	else
	{
		*val_ptr = append_char(*val_ptr, current_char_val, ctx);
		if (!*val_ptr)
			return (-1);
		(*(params->idx_ptr))++;
	}
	return (1);
}

/*
** Helper function for get_quoted_val to process characters within quotes.
** Handles '$?' expansion and appends characters to accumulated_value.
** Updates 'i' to the current processing position.
** Returns:
**  0: If a $ expansion occurred and the main loop should re-evaluate or
continue. (Not used by this func)
**  1: If a literal character was appended and the main loop should continue.
** -1: On critical error (e.g., malloc).
*/
int	process_char_in_quoted_val(t_char_iter_params *params,
								char **accumulated_value,
								t_context *ctx)
{
	char	*temp_val;
	int		status;

	temp_val = *accumulated_value;
	if (params->q_char == '"')
	{
		status = process_double_quote_char_val(params, &temp_val, ctx);
		if (status == -1)
			return (-1);
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
