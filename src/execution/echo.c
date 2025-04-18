/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:10:03 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/18 10:52:47 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
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

void	echo(char **args)
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
