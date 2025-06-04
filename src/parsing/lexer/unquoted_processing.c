/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquoted_processing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 00:00:00 by AI                #+#    #+#             */
/*   Updated: 2025/06/04 18:31:37 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	process_extracted_env_var(char **accumulated_value,
										char *var_name,
										t_context *ctx)
{
	char	*var_value;
	char	*temp_val;

	temp_val = *accumulated_value;
	var_value = get_env_value(ctx->envp, var_name);
	free(var_name);
	if (var_value)
	{
		temp_val = append_extracted(temp_val, var_value, ctx);
		if (!temp_val)
			return (-1);
	}
	*accumulated_value = temp_val;
	return (1);
}

static int	handle_unquoted_env_var(const char *command, int *i,
								char **accumulated_value,
								t_context *ctx)
{
	char	*var_name;
	int		original_i_val;

	original_i_val = *i;
	(*i)++;
	var_name = extract_env_var_name(command, i);
	if (var_name)
	{
		return (process_extracted_env_var(accumulated_value, var_name, ctx));
	}
	else
	{
		*i = original_i_val;
		return (handle_unquoted_regular_char(command, i, accumulated_value,
				ctx));
	}
}

/*
** Processes a single character in an unquoted context.
** Handles backslash escapes, '$?' expansion, general env var expansion,
** or appends a regular character.
** Updates the parsing index `i` and the `accumulated_value`.
** Returns:
** - 0 if '$?' was expanded (special case for loop control in caller).
** - 1 on successful processing of a regular char, escaped char or env var.
** - -1 on malloc error.
*/
int	process_char_in_unquoted_val(const char *command, int *i,
		char **accumulated_value, t_context *ctx)
{
	if (command[*i] == '\\')
	{
		return (process_backslash_unquoted(command, i, accumulated_value, ctx));
	}
	else if (command[*i] == '$')
	{
		if (command[(*i) + 1] == '?')
			return (handle_unquoted_dollar_q(i, accumulated_value, ctx));
		else if (is_valid_var_char_start(command[(*i) + 1]))
			return (handle_unquoted_env_var(command, i,
					accumulated_value, ctx));
		else
			return (handle_unquoted_regular_char(command, i, accumulated_value,
					ctx));
	}
	else
		return (handle_unquoted_regular_char(command, i, accumulated_value,
				ctx));
}
