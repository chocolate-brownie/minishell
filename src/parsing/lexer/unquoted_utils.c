/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquoted_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 18:06:55 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/04 19:03:19 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	process_backslash_unquoted(const char *command, int *i,
		char **accumulated_value, t_context *ctx)
{
	char	*temp_val;

	temp_val = *accumulated_value;
	if (command[(*i) + 1] != '\0')
	{
		temp_val = append_char(temp_val, command[(*i) + 1], ctx);
		if (!temp_val)
			return (-1);
		*i += 2;
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

int	handle_unquoted_dollar_q(int *i,
									char **accumulated_value,
									t_context *ctx)
{
	char	*temp_val;

	temp_val = *accumulated_value;
	temp_val = expand_dollar_question(temp_val, i, ctx);
	if (!temp_val)
		return (-1);
	*accumulated_value = temp_val;
	return (0);
}

int	handle_unquoted_regular_char(const char *command, int *i,
		char **accumulated_value, t_context *ctx)
{
	char	*temp_val;

	temp_val = *accumulated_value;
	temp_val = append_char(temp_val, command[*i], ctx);
	if (!temp_val)
		return (-1);
	(*i)++;
	*accumulated_value = temp_val;
	return (1);
}
