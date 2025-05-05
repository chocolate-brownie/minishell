/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:28:55 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/04 19:01:00 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_state(int argc, char *argv[])
{
	if (argc > 1 || !argv[0])
	{
		perror("The program accepts no arguments\n");
		return (1);
	}
	if (clear_term() != 0)
		return (1);
	return (0);
}

void	set_null(int count, ...)
{
	va_list	args;
	int		i;
	void	**ptr;

	va_start(args, count);
	i = 0;
	while (i < count)
	{
		ptr = va_arg(args, void **);
		*ptr = NULL;
	}
	va_end(args);
}
