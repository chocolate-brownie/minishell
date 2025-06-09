/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_manager.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:08:54 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/09 18:58:35 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	setup_heredoc_core_data(t_token **curr_token_ptr,
									t_heredoc_data *hdata,
									t_context *ctx,
									t_token **out_delimiter_token)
{
	t_token	*operator_token;

	init_heredoc_struct(hdata);
	operator_token = *curr_token_ptr;
	*out_delimiter_token = operator_token->next;
	if (heredoc_delim_validation(ctx, *out_delimiter_token) != 0)
		return (-1);
	hdata->delimiter_str = (*out_delimiter_token)->value;
	return (0);
}

static int	populate_heredoc_tempfile(t_heredoc_data *hdata, t_context *ctx)
{
	int	read_status;

	hdata->fd = hd_tempfile(ctx, ctx->heredoc_counter, &hdata->temp_filepath);
	if (hdata->fd < 0)
		return (-1);
	read_status = read_heredoc_input(hdata, ctx);
	if (hdata->fd != -1)
	{
		if (close(hdata->fd) == -1 && read_status != 0 && g_signal != SIGINT)
		{
			perror("minishell: heredoc close");
			set_exit_code(ctx, 1, hdata->temp_filepath);
			read_status = -1;
		}
		hdata->fd = -1;
	}
	return (read_status);
}

static void	advance_past_invalid_delim(t_token **curr_token_ptr)
{
	if (*curr_token_ptr)
		*curr_token_ptr = (*curr_token_ptr)->next;
	if (*curr_token_ptr)
		*curr_token_ptr = (*curr_token_ptr)->next;
}

/** Handles the entire heredoc process..
gets delimeter, creates temp files and read input until delimiter EOF
encounters and writes to temp file

@param current_token_ptr points to the current token (should be the <<)
@param ctx points to the shell's context struct
@return char * path to the temp file containing heredoc input on success.

The caller is responsible for freesing this path and unlinking the file
when its no longer needed. RETURNS NULL of failure (eg. syntax error, will be
advanced past the delimiter token on success malloc error, ctrol-c interruption
, file I/O error */
char	*handle_heredoc(t_token **curr_token_ptr, t_context *ctx)
{
	t_heredoc_data	hdata;
	t_token			*delimiter_token;
	int				read_status;

	if (g_signal == SIGINT)
		return (NULL);
	if (ctx && ctx->has_syntax_error)
		return (NULL);
	if (setup_heredoc_core_data(curr_token_ptr, &hdata, ctx,
			&delimiter_token) != 0)
	{
		advance_past_invalid_delim(curr_token_ptr);
		return (NULL);
	}
	read_status = populate_heredoc_tempfile(&hdata, ctx);
	if (g_signal == SIGINT || read_status < 0)
		return (cleanup_hd_resources(&hdata), NULL);
	add_active_heredoc(ctx, hdata.temp_filepath);
	*curr_token_ptr = delimiter_token->next;
	ctx->heredoc_counter++;
	return (hdata.temp_filepath);
}
