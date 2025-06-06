/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoted_val_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 20:30:00 by ai-assistan       #+#    #+#             */
/*   Updated: 2025/06/06 12:49:46 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*
** Handles the closing of a quote, setting position and returning value.
*/
static char	*handle_quote_closure(t_quote_finish_params *params,
									int *position,
									char *accumulated_value,
									t_context *ctx)
{
	if (params->cmd_str[params->idx] == params->q_char)
	{
		*position = params->idx + 1;
		return (accumulated_value);
	}
	else
	{
		free(accumulated_value);
		set_exit_code(ctx, ERR_UNCLOSED_QUOTE, NULL);
		return (NULL);
	}
}

/*
** Helper function to process the main loop for get_quoted_val.
** Iterates through characters within quotes, handles expansions, and appends.
** Modifies *accumulated_value and uses iter_params->idx_ptr to update index.
** Returns 0 on successful loop completion, -1 on error.
*/
static int	process_quoted_string_loop(t_char_iter_params *iter_params,
										char **accumulated_value,
										t_context *ctx)
{
	int		process_status_inner;
	char	current_char;

	while (1)
	{
		current_char = iter_params->cmd_str[*(iter_params->idx_ptr)];
		if (current_char == '\0' || current_char == iter_params->q_char)
			break ;
		process_status_inner = process_char_in_quoted_val(iter_params,
				accumulated_value, ctx);
		if (process_status_inner == -1)
			return (-1);
		else if (process_status_inner == 0)
			continue ;
	}
	return (0);
}

/*
** Initializes parameters for get_quoted_val.
** Sets up quote_char, index i, accumulated_value, and iter_params.
** Returns 0 on success, -1 on error (e.g., malloc failure, invalid input).
*/
static int	init_quoted_val_extraction(t_quote_extract_init_params *init_data,
										t_context *ctx)
{
	if (!init_data->command || !init_data->position || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), -1);
	*(init_data->quote_char_out) = init_data->command[*(init_data->position)];
	*(init_data->i_out) = *(init_data->position) + 1;
	*(init_data->accumulated_value_out) = ft_strdup("");
	if (!(*(init_data->accumulated_value_out)))
		return (set_exit_code(ctx, ERR_MALLOC, "init_quoted_val"), -1);
	init_data->iter_params_out->cmd_str = init_data->command;
	init_data->iter_params_out->idx_ptr = init_data->i_out;
	init_data->iter_params_out->q_char = *(init_data->quote_char_out);
	return (0);
}

/*
** Extracts the content of a quoted segment from `command` starting
    at `*position`.
** Handles expansion of `$?` within double quotes.
** Updates `*position` to point after the consumed quoted segment.
** Returns the extracted (and possibly expanded) string, or NULL on error or
** if quotes are unclosed.
*/
char	*get_quoted_val(const char *command, int *position, t_context *ctx)
{
	t_quote_val_params			params;
	t_char_iter_params			iter_params;
	t_quote_finish_params		finish_params;
	t_quote_extract_init_params	init_data;

	init_data.command = command;
	init_data.position = position;
	init_data.accumulated_value_out = &params.accumulated_value;
	init_data.iter_params_out = &iter_params;
	init_data.i_out = &params.i;
	init_data.quote_char_out = &params.quote_char;
	if (init_quoted_val_extraction(&init_data, ctx) == -1)
		return (NULL);
	params.loop_status = process_quoted_string_loop(&iter_params,
			&params.accumulated_value, ctx);
	if (params.loop_status == -1)
		return (free(params.accumulated_value), NULL);
	finish_params.cmd_str = command;
	finish_params.idx = params.i;
	finish_params.q_char = params.quote_char;
	return (handle_quote_closure(&finish_params, position,
			params.accumulated_value, ctx));
}
