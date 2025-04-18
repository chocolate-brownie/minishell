/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:46:37 by shasinan          #+#    #+#             */
/*   Updated: 2025/04/18 10:52:30 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	char	**args;

	(void)ac;
	(void)av;
	(void)env;
	while (1)
	{
		line = readline("minishell>");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		args = ft_split(line, ' ');
		if (args && args[0] && (strcmp(args[0], "echo") == 0))
			echo(args);
		free(line);
		free_tab(args);
	}
	return (0);
}
 */