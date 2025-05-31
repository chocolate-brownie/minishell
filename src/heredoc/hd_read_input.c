/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_read_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 00:39:07 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/31 20:06:10 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	perform_read_loop(char *buffer, int *read_len)
{
	char	c;
	int		ret;
	int		current_len;

	current_len = 0;
	while (current_len < 1023)
	{
		ret = read(STDIN_FILENO, &c, 1);
		if (g_signal == SIGINT)
			return (1);
		if (ret == 0)
		{
			*read_len = current_len;
			return (2);
		}
		if (c == '\n')
		{
			*read_len = current_len;
			return (0);
		}
		buffer[current_len++] = c;
	}
	*read_len = current_len;
	return (0);
}

static char	*read_line_manual(t_context *ctx)
{
	char	*buffer;
	int		len;
	int		loop_status;

	write(STDOUT_FILENO, "heredoc> ", 9);
	buffer = malloc(1024);
	if (!buffer)
	{
		set_exit_code(ctx, ERR_MALLOC, "malloc at read_line_manual");
		return (NULL);
	}
	len = 0;
	loop_status = perform_read_loop(buffer, &len);
	if (loop_status == 1)
	{
		free(buffer);
		return (NULL);
	}
	if (loop_status == 2 && len == 0)
	{
		free(buffer);
		return (NULL);
	}
	buffer[len] = '\0';
	return (buffer);
}

static int	process_single_heredoc_line(char *line_read,
										t_heredoc_data *hdata,
										t_context *ctx)
{
	size_t	len_line;
	size_t	len_delim;

	if (!line_read)
		return (0);
	len_line = ft_strlen(line_read);
	len_delim = ft_strlen(hdata->delimiter_str);
	if (len_line == len_delim && ft_strncmp(line_read, hdata->delimiter_str,
			len_delim) == 0)
		return (free(line_read), 0);
	if (write(hdata->fd, line_read, len_line) == -1)
	{
		set_exit_code(ctx, ERR_PIPE, "heredoc write failed");
		return (free(line_read), -1);
	}
	if (write(hdata->fd, "\n", 1) == -1)
	{
		set_exit_code(ctx, ERR_PIPE, "heredoc write failed");
		return (free(line_read), -1);
	}
	return (free(line_read), 1);
}

static int	heredoc_read_and_process_loop(t_heredoc_data *hdata,
											t_context *ctx,
											struct sigaction *sa_old_int)
{
	char	*line_read;
	int		process_rc;

	while (1)
	{
		line_read = read_line_manual(ctx);
		if (g_signal == SIGINT)
			return (handle_heredoc_interrupt(line_read, sa_old_int, ctx));
		if (!line_read)
		{
			restore_old_signals(sa_old_int);
			return (handle_eof_case(hdata));
		}
		process_rc = process_single_heredoc_line(line_read, hdata, ctx);
		if (process_rc <= 0)
		{
			restore_old_signals(sa_old_int);
			return (process_rc);
		}
	}
}

int	read_heredoc_input(t_heredoc_data *hdata, t_context *ctx)
{
	struct sigaction	sa_heredoc_int;
	struct sigaction	sa_old_int;
	int					status;

	setup_heredoc_signals(&sa_heredoc_int, &sa_old_int);
	status = heredoc_read_and_process_loop(hdata, ctx, &sa_old_int);
	restore_old_signals(&sa_old_int);
	return (status);
}
