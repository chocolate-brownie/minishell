/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_to_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:21:23 by shasinan          #+#    #+#             */
/*   Updated: 2025/06/03 11:44:19 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_arg(t_exec *cmd, int include_cmd_name)
{
	int		count;
	t_args	*args;

	count = include_cmd_name;
	args = cmd->args;
	while (args)
	{
		count++;
		args = args->next;
	}
	return (count);
}

static int	malloc_arr(t_exec *cmd, int include_cmd_name, char ***arr)
{
	int	len;

	len = count_arg(cmd, include_cmd_name);
	*arr = malloc(sizeof(char *) * (len + 1));
	if (!*arr)
		return (0);
	return (1);
}

static void	free_if_fail(char **arr, int i)
{
	while (--i >= 0)
		free(arr[i]);
	free(arr);
}

static int	fill_arg_array(char **arr, t_args *args, int i)
{
	t_args	*a;

	a = args;
	while (a)
	{
		if (a->value)
		{
			arr[i] = ft_strdup(a->value);
			if (!arr[i])
			{
				free_if_fail(arr, i);
				return (0);
			}
			i++;
		}
		a = a->next;
	}
	arr[i] = NULL;
	return (1);
}

char	**args_to_array(t_exec *cmd, int include_cmd_name)
{
	char	**arr;
	int		i;

	if (!malloc_arr(cmd, include_cmd_name, &arr))
		return (NULL);
	i = 0;
	if (include_cmd_name)
	{
		arr[i] = ft_strdup(cmd->cmd);
		if (!arr[i])
		{
			free_if_fail(arr, i);
			return (NULL);
		}
		i++;
	}
	if (!fill_arg_array(arr, cmd->args, i))
		return (NULL);
	return (arr);
}
