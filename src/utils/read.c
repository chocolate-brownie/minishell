/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:55:23 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/27 15:27:13 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*changes added :
	-setup_signal_parent();
	-check NULL before "if (command[0] == '\0'")
	-check the global variable*/
char	*read_cmd(t_context *ctx)
{
	char	*command;

	while (1)
	{
		setup_signal_parent();
		command = readline(YELLOW PROMPT RESET);
		if (!command)
			return (NULL);
		if (command[0] == '\0')
		{
			if (g_signal)
			{
				ctx->last_exit_code = 130;
				g_signal = 0;
			}
			free(command);
			continue ;
		}
		add_history(command);
		return (command);
	}
}
