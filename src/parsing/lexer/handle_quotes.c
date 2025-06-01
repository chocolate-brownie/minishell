/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:18:46 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/01 21:00:53 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*
** Finalizes the extraction of an unquoted value.
*/
static char	*finalize_unquoted_extraction(char *accumulated_value,
											int start_idx,
											int current_idx)
{
	if (ft_strlen(accumulated_value) == 0 && current_idx == start_idx)
	{
		free(accumulated_value);
		return (NULL);
	}
	return (accumulated_value);
}

/*
** Extracts an unquoted segment from `command` starting at `*position`.
** An unquoted segment continues until a delimiter or a quote is encountered.
** Handles expansion of `$?`.
** Updates `*position` to point after the consumed unquoted segment.
** Returns the extracted (and possibly expanded) string, or NULL if no unquoted
** segment is found or an error occurs.
*/
char	*get_unquoted_val(const char *command, int *position, t_context *ctx)
{
	int		start_idx;
	char	*accumulated_value;
	int		i;
	int		process_status;

	if (!command || !position || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), NULL);
	start_idx = *position;
	i = start_idx;
	accumulated_value = ft_strdup("");
	if (!accumulated_value)
		return (set_exit_code(ctx, ERR_MALLOC, ERMSG_STRDUP_UQVAL), NULL);
	while (command[i] && !is_delimiter(command[i]) && !is_quote(command[i]))
	{
		process_status = process_char_in_unquoted_val(command, &i,
				&accumulated_value, ctx);
		if (process_status == -1)
			return (free(accumulated_value), NULL);
		else if (process_status == 0)
			continue ;
	}
	*position = i;
	return (finalize_unquoted_extraction(accumulated_value, start_idx, i));
}
