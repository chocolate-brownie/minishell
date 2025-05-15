/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:55:23 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/15 16:04:33 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*read_cmd(void)
{
	char	*command;

	while (1)
	{
		command = readline(YELLOW PROMPT RESET);
		if (command[0] == '\0')
		{
			free(command);
			continue ;
		}
		add_history(command);
		return (command);
	}
}
