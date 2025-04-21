/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:10:03 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/21 19:05:06 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tab(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

int	valid_option(char **args, int i)
{
	int	j;

	if (args[i][0] != '-')
		return (0);
	j = 1;
	while (args[i][j])
	{
		if (args[i][j] == 'n')
			j++;
		else
			break ;
	}
	if (args[i][j] == '\0')
		return (1);
	else
		return (0);
}

void	ft_echo(char **args)
{
	int	i;
	int	option;

	i = 1;
	option = 0;
	while (args[i] && valid_option(args, i) == 1)
	{
		option = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!option)
		write(1, "\n", 1);
}

/*
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
