/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:00:29 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/08 16:13:58 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_eof_case(t_heredoc_data *hdata)
{
	ft_putstr_fd("\nminishell: warning: here-document ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted \'", STDERR_FILENO);
	ft_putstr_fd(hdata->delimiter_str, STDERR_FILENO);
	return (ft_putstr_fd("\')\n", STDERR_FILENO), 0);
}

int	heredoc_delim_validation(t_context *ctx, t_token *delim_token)
{
	if (delim_token == NULL || delim_token->type == TOKEN_EOF)
	{
		set_exit_code(ctx, ERR_SYNTAX, "newline");
		return (-1);
	}
	if (delim_token->type != TOKEN_WORD)
	{
		set_exit_code(ctx, ERR_SYNTAX, delim_token->value);
		return (-1);
	}
	return (0);
}

void	init_heredoc_struct(t_heredoc_data *hdata)
{
	if (!hdata)
		return ;
	hdata->delimiter_str = NULL;
	hdata->temp_filepath = NULL;
	hdata->fd = -1;
	return ;
}

int	hd_tempfile(t_context *ctx, int count, char **path_out)
{
	char	*count_str;
	int		fd;

	count_str = ft_itoa(count);
	if (!count_str)
		return (set_exit_code(ctx, ERR_MALLOC, ERMSG_HEREDOC_ITOA), -1);
	*path_out = NULL;
	*path_out = ft_strjoin(HEREDOC_FILE_PREFIX, count_str);
	free(count_str);
	if (!*path_out)
		return (set_exit_code(ctx, ERR_MALLOC, ERMSG_HEREDOC_STRJOIN), -1);
	fd = open(*path_out, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("minishell: heredoc tempfile open");
		set_exit_code(ctx, 1, *path_out);
		free(*path_out);
		*path_out = NULL;
		return (-1);
	}
	return (fd);
}

void	cleanup_hd_resources(t_heredoc_data *hdata)
{
	if (hdata->fd > 0)
	{
		close(hdata->fd);
		hdata->fd = -1;
	}
	if (hdata->temp_filepath)
	{
		unlink(hdata->temp_filepath);
		free(hdata->temp_filepath);
		hdata->temp_filepath = NULL;
	}
}
