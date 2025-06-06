/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 23:04:26 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/11 00:21:23 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	*accumulate_word_value(const char *cmd, int start_pos,
		int *final_pos_ptr, t_context *ctx)
{
	int		curr_pos;
	char	*accumulated_value;
	char	*segment_value;
	char	*temp_accumulated;

	if (!cmd || !final_pos_ptr || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), NULL);
	curr_pos = start_pos;
	accumulated_value = ft_strdup("");
	if (!accumulated_value)
		return (set_exit_code(ctx, ERR_MALLOC, NULL), NULL);
	while (cmd[curr_pos] && !is_delimiter(cmd[curr_pos]))
	{
		segment_value = handle_quotes(cmd, &curr_pos, ctx);
		if (!segment_value)
			return (free(accumulated_value), NULL);
		temp_accumulated = append_extracted(accumulated_value, segment_value,
				ctx);
		if (!temp_accumulated)
			return (free(accumulated_value), NULL);
		accumulated_value = temp_accumulated;
	}
	*final_pos_ptr = curr_pos;
	return (accumulated_value);
}

t_token	*handle_word(const char *cmd, int *i, t_context *ctx)
{
	t_token	*final_token;
	char	*accumulated_value;
	int		final_pos;

	if (!cmd || !i || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), NULL);
	final_pos = *i;
	accumulated_value = accumulate_word_value(cmd, *i, &final_pos, ctx);
	if (!accumulated_value)
		return (NULL);
	final_token = create_token(accumulated_value, TOKEN_WORD);
	free(accumulated_value);
	if (!final_token)
		return (set_exit_code(ctx, ERR_MALLOC, NULL), NULL);
	*i = final_pos;
	return (final_token);
}
