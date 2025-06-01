/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_string_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:00:00 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/01 21:07:10 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*
** Appends `value` to `accumulated_value`.
** Frees both `accumulated_value` and `value` if the join is successful.
** If `value` is NULL, `accumulated_value` is returned unchanged.
** If `accumulated_value` is NULL, `value` is returned (ownership transferred).
** Returns the new concatenated string, or NULL on malloc failure 
** (sets exit code).
*/
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
		return (set_exit_code(ctx, ERR_MALLOC, "strjoin in append_extracted"),
			NULL);
	return (temp);
}

/*
** Handles the extraction of a single segment from the command string `cmd`
** starting at `*index`. This segment can be either quoted or unquoted.
** Updates `*index` to the position after the extracted segment.
** Returns the extracted string (potentially expanded), or NULL if no segment
** is extracted or an error occurs.
*/
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
	*index = curr_pos;
	return (value);
}
