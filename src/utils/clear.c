/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:11:41 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/25 19:55:13 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	my_putchar(int c)
{
	return (write(1, &c, 1));
}

static int	setup_termcap(char **term_buffer_ptr)
{
	char	*term_type;
	int		status;

	term_type = getenv("TERM");
	if (!term_type)
	{
		perror("getenv(TERM) failed");
		return (-1);
	}
	*term_buffer_ptr = malloc(2048);
	if (!(*term_buffer_ptr))
	{
		perror("malloc failed for termcap buffer");
		return (-1);
	}
	status = tgetent(*term_buffer_ptr, term_type);
	if (status <= 0)
	{
		if (status == 0)
			write(2, "Termcap entry not found.\n", 26);
		else
			perror("tgetent failed");
		return (-1);
	}
	return (0);
}

static char	*get_clear_string(char *tc_area, size_t tc_area_size)
{
	char	*clear_str;
	char	*tc_ptr;

	tc_ptr = tc_area;
	if (tc_area_size > 0)
		tc_area[0] = '\0';
	clear_str = tgetstr("cl", &tc_ptr);
	if (!clear_str)
	{
		write(2, "Clear capability ('cl') not found.\n", 36);
		return (NULL);
	}
	if ((size_t)(tc_ptr - tc_area) >= tc_area_size)
	{
		write(2, "Termcap tc_area buffer overrun.\n", 33);
		return (NULL);
	}
	return (clear_str);
}

int	clear_term(void)
{
	char	*term_buffer;
	char	*clear_str;
	char	tc_area[1024];
	int		ret;

	term_buffer = NULL;
	ret = -1;
	if (setup_termcap(&term_buffer) == 0)
	{
		clear_str = get_clear_string(tc_area, sizeof(tc_area));
		if (clear_str)
		{
			if (tputs(clear_str, 1, my_putchar) != ERR)
				ret = 0;
			else
				write(2, "tputs failed to output clear string.\n", 38);
		}
	}
	if (term_buffer)
		free(term_buffer);
	return (ret);
}
