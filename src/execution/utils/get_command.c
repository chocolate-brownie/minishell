/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 17:52:26 by shasinan          #+#    #+#             */
/*   Updated: 2025/06/04 09:25:09 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**free_tabl(char **tabl)
{
	int	i;

	i = 0;
	while (tabl[i])
		free(tabl[i++]);
	free(tabl);
	return (NULL);
}

static char	*get_env_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

int	is_directory(const char *path)
{
	struct stat st;

	if (stat(path, &st) != 0)
		return (0);
	return (S_ISDIR(st.st_mode));
}

static char	*paths_loop(char **paths, char *cmd)
{
	char	*tmp;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full_path)
		{
			free_tabl(paths);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0 && !is_directory(full_path))
		{
			free_tabl(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_tabl(paths);
	return (NULL);
}

char	*get_cmd_path(char **envp, char *cmd)
{
	char	*env_path;
	char	*path;
	char	**paths;
	char	*full_path;

	env_path = get_env_path(envp);
	if (!env_path)
		return (NULL);
	path = ft_strjoin(env_path, ":.");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
	{
		free(path);
		return (NULL);
	}
	full_path = paths_loop(paths, cmd);
	free(path);
	return (full_path);
}
