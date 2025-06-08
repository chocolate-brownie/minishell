/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_char_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 16:13:00 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/08 16:25:55 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_char_output(char c, int *current_len)
{
	if (c >= 32 && c <= 126)
		write(STDOUT_FILENO, &c, 1);
	else if (c == '\t')
		write(STDOUT_FILENO, "\t", 1);
	else if (c == 127 && *current_len > 0)
	{
		write(STDOUT_FILENO, "\b \b", 3);
		(*current_len)--;
	}
}

static int	consume_escape_sequence(void)
{
	char	c;
	int		ret;

	ret = read(STDIN_FILENO, &c, 1);
	if (ret <= 0)
		return (1);
	if (c == '[')
	{
		ret = read(STDIN_FILENO, &c, 1);
		if (ret <= 0)
			return (1);
	}
	return (1);
}

int	process_char(char c, char *buffer, int *current_len)
{
	if (c == '\n')
		return (write(STDOUT_FILENO, "\n", 1), 0);
	if (c == 4 && *current_len == 0)
		return (-1);
	if (c == 127 && *current_len == 0)
		return (1);
	if (c == 127)
		return (handle_char_output(c, current_len), 1);
	if (c == 27)
		return (consume_escape_sequence());
	if (c < 32 && c != '\t')
		return (1);
	handle_char_output(c, current_len);
	buffer[(*current_len)++] = c;
	return (1);
}
