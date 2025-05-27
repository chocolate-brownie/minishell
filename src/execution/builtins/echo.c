/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:10:03 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/17 10:02:25 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tab(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

static int	valid_option(char **args, int i)
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

static void	putstr_args(char **args, int i)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
}

int	ft_echo(t_exec *cmd)
{
	int		i;
	int		option;
	char	**args;

	args = args_to_array(cmd, 0);
	if (!args)
		return (1);
	i = 0;
	option = 0;
	while (args[i] && valid_option(args, i) == 1)
	{
		option = 1;
		i++;
	}
	putstr_args(args, i);
	if (!option)
		write(1, "\n", 1);
	free_tab(args);
	return (0);
}
