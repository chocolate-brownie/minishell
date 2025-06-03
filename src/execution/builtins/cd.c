/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:37:56 by shasinan          #+#    #+#             */
/*   Updated: 2025/06/02 12:53:31 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	change_dir(char *final_path)
{
	if (chdir(final_path) == -1)
	{
		perror(final_path);
		return (0);
	}
	return (1);
}

static char	*get_path(t_exec *cmd, t_env *env)
{
	char	*final_path;

	if (cmd->args && cmd->args->next)
		return (ft_putstr_fd("cd : too many arguments\n", 2), NULL);
	if (!cmd->args || !cmd->args->value || cmd->args->value[0] == '\0'
		|| !ft_strcmp(cmd->args->value, "~") || !ft_strcmp(cmd->args->value,
			"--"))
	{
		final_path = get_env_value(env, "HOME");
		if (!final_path)
		{
			ft_putstr_fd("$HOME not set\n", 2);
			return (NULL);
		}
	}
	else if (!ft_strcmp(cmd->args->value, "-"))
	{
		final_path = get_env_value(env, "OLDPWD");
		if (!final_path)
			return (ft_putstr_fd("$OLDPWD not set\n", 2), NULL);
		printf("%s\n", final_path);
	}
	else
		final_path = ft_strdup(cmd->args->value);
	return (final_path);
}

int	ft_cd(t_exec *cmd, t_env *env)
{
	char	*final_path;
	char	*oldpwd_value;
	char	pwd_value[4096];

	final_path = get_path(cmd, env);
	if (!final_path)
		return (1);
	oldpwd_value = get_env_value(env, "PWD");
	if (!oldpwd_value)
		return (free(final_path), 1);
	if (!change_dir(final_path))
		return (free(final_path), free(oldpwd_value), 1);
	if (!getcwd(pwd_value, 4096))
	{
		free(oldpwd_value);
		free(final_path);
		perror("getcwd");
		return (1);
	}
	update_env_var(env, "OLDPWD", oldpwd_value);
	update_env_var(env, "PWD", pwd_value);
	free(oldpwd_value);
	free(final_path);
	return (0);
}
