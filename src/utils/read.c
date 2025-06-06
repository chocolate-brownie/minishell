/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:55:23 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/06 18:55:38 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*read_cmd(t_context *ctx)
{
	char	*command;

	while (1)
	{
		setup_signal_parent();
		command = readline(YELLOW PROMPT RESET);
		if (!command)
			return (NULL);
		if (g_signal)
		{
			ctx->last_exit_code = 130;
			g_signal = 0;
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
