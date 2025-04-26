/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:55:23 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/25 17:18:19 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_exit_condition(char *command)
{
	if (command == NULL)
	{
		ft_putstr_fd("exit\n", 2);
		return (1);
	}
	if (ft_strlen(command) == 4 && ft_strncmp("exit", command, 4) == 0)
		return (1);
	return (0);
}

char	*read_cmd(void)
{
	char	*command;

	while (1)
	{
		command = readline(PROMPT);
		if (check_exit_condition(command) == 1)
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
