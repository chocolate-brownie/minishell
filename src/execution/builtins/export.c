/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:05:54 by shasinan          #+#    #+#             */
/*   Updated: 2025/06/04 09:48:08 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_id(char *args)
{
	int	i;

	if (ft_isdigit(args[0]))
		return (0);
	i = 0;
	while (args[i] && args[i] != '=')
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	handle_export_arg(t_env *env, char *arg)
{
	int		j;
	char	*id;
	char	*value;

	j = 0;
	while (arg[j] && arg[j] != '=')
		j++;
	id = ft_strndup(arg, j);
	if (!id)
		return (0);
	if (!valid_id(arg) || id[0] == '\0')
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (free(id), 0);
	}
	if (!ft_strchr(arg, '='))
		return (free(id), 1);
	value = ft_strdup(arg + j + 1);
	if (!value)
		return (free(id), 0);
	if (!update_env_var(env, id, value))
		return (free(id), free(value), 0);
	return (free(id), free(value), 1);
}

int	ft_export(t_env *env, t_exec *cmd)
{
	int		i;
	char	**args;
	int		invalid_found;

	invalid_found = 0;
	args = args_to_array(cmd, 0);
	if (!args)
		return (1);
	i = 0;
	while (args[i])
	{
		if (!handle_export_arg(env, args[i]))
			invalid_found = 1;
		i++;
	}
	free_tab(args);
	if (invalid_found)
		return (1);
	else
		return (0);
}
