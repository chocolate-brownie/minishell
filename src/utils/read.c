/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:55:23 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/26 17:36:56 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*read_cmd(void)
{
	char	*command;

	rl_catch_signals = 0;
	command = readline(YELLOW PROMPT RESET);
	if (command == NULL)
		return (NULL);
	if (command[0] == '\0')
	{
		free(command);
		return (ft_strdup(""));
	}
	return (command);
}
