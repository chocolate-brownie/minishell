/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_env_var_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:37:05 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/04 17:37:07 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*
** Checks if a character is a valid starting character for an environment
** variable name (alphabetic or underscore).
*/
int	is_valid_var_char_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

/*
** Checks if a character is a valid subsequent character for an environment
** variable name (alphanumeric or underscore).
*/
int	is_valid_var_char_subsequent(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/*
** Extracts an environment variable name from the command string.
** - cmd_str: The full command string.
** - idx_ptr: Pointer to the current index in cmd_str. On entry, this should
**            point to the first character of the variable name (i.e.,
**            the character immediately following a '$').
** On exit, *idx_ptr is advanced past the extracted variable name.
** Returns a newly allocated string containing the variable name, or NULL
** if no valid variable name is found or if memory allocation fails.
*/
char	*extract_env_var_name(const char *cmd_str, int *idx_ptr)
{
	int		start_pos;
	int		len;
	char	*var_name;

	if (!cmd_str || !idx_ptr)
		return (NULL);
	start_pos = *idx_ptr;
	len = 0;
	if (!is_valid_var_char_start(cmd_str[start_pos]))
		return (NULL);
	len++;
	while (cmd_str[start_pos + len]
		&& is_valid_var_char_subsequent(cmd_str[start_pos + len]))
	{
		len++;
	}
	if (len == 0)
		return (NULL);
	var_name = ft_substr(cmd_str, start_pos, len);
	if (!var_name)
		return (NULL);
	*idx_ptr = start_pos + len;
	return (var_name);
}

int	handle_backslash_dq(t_char_iter_params *params,
								char **val_ptr,
								t_context *ctx)
{
	char	next_char_val;
	char	current_char_val;

	current_char_val = params->cmd_str[*(params->idx_ptr)];
	next_char_val = params->cmd_str[*(params->idx_ptr) + 1];
	if (next_char_val == '$' || next_char_val == '"' || next_char_val == '\\')
	{
		*val_ptr = append_char(*val_ptr, next_char_val, ctx);
		(*(params->idx_ptr)) += 2;
	}
	else
	{
		*val_ptr = append_char(*val_ptr, current_char_val, ctx);
		(*(params->idx_ptr))++;
	}
	if (!*val_ptr)
		return (-1);
	return (1);
}

/*
** Appends a single character `c` to string `str`.
** Uses `append_extracted` for the core logic.
** Frees original `str` and returns new string or NULL on error.
*/
char	*append_char(char *str, char c, t_context *ctx)
{
	char	add[2];
	char	*new_str;

	add[0] = c;
	add[1] = '\0';
	new_str = append_extracted(str, ft_strdup(add), ctx);
	return (new_str);
}
