/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:55:23 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/11 00:39:18 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_exit_condition(char *command, t_context *ctx)
{
	if (command == NULL)
	{
		set_exit_code(ctx, 0, NULL);
		ft_putstr_fd("exit\n", 2);
		return (1);
	}
	if (ft_strlen(command) == 4 && ft_strncmp("exit", command, 4) == 0)
	{
		set_exit_code(ctx, 0, NULL);
		return (1);
	}
	return (0);
}

char	*read_cmd(t_context *ctx)
{
	char	*command;

	while (1)
	{
		command = readline(YELLOW PROMPT RESET);
		if (check_exit_condition(command, ctx) == 1)
		{
			if (command != NULL)
				free(command);
			return (NULL);
		}
		if (command[0] == '\0')
		{
			free(command);
			continue ;
		}
		add_history(command);
		return (command);
	}
}
