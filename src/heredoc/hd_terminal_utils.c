/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_terminal_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:45:00 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/08 17:39:40 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	setup_terminal_mode(struct termios *old_term, struct termios *new_term)
{
	if (tcgetattr(STDIN_FILENO, old_term) == 0)
	{
		*new_term = *old_term;
		new_term->c_lflag &= ~(ICANON | ECHO | ECHOCTL);
		new_term->c_cc[VMIN] = 1;
		new_term->c_cc[VTIME] = 0;
		tcsetattr(STDIN_FILENO, TCSANOW, new_term);
		return (1);
	}
	return (0);
}

void	restore_terminal_mode(struct termios *old_term)
{
	struct termios	current_term;

	if (tcgetattr(STDIN_FILENO, &current_term) == 0)
		tcsetattr(STDIN_FILENO, TCSANOW, old_term);
}

static int	write_expanded_line_to_fd(char *expanded_line, int fd)
{
	if (write(fd, expanded_line, ft_strlen(expanded_line)) == -1)
		return (-1);
	if (write(fd, "\n", 1) == -1)
		return (-1);
	return (0);
}

int	process_single_heredoc_line(char *line_read, t_heredoc_data *hdata,
								t_context *ctx)
{
	size_t	len_line;
	size_t	len_delim;
	char	*expanded_line;

	if (!line_read)
		return (0);
	len_line = ft_strlen(line_read);
	len_delim = ft_strlen(hdata->delimiter_str);
	if (len_line == len_delim && ft_strncmp(line_read, hdata->delimiter_str,
			len_delim) == 0)
		return (free(line_read), 0);
	expanded_line = expand_heredoc_line(line_read, ctx);
	free(line_read);
	if (!expanded_line)
		return (set_exit_code(ctx, ERR_MALLOC, "heredoc expand"), -1);
	if (write_expanded_line_to_fd(expanded_line, hdata->fd) == -1)
	{
		set_exit_code(ctx, ERR_PIPE, "heredoc write failed");
		return (free(expanded_line), -1);
	}
	return (free(expanded_line), 1);
}
