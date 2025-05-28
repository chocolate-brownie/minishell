/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 10:16:59 by shasinan          #+#    #+#             */
/*   Updated: 2025/05/28 19:27:08 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_size(t_env *env)
{
	int		count;
	t_env	*tmp;

	tmp = env;
	count = 0;
	while (tmp)
	{
		if (tmp->raw)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**env_to_envp(t_env *env)
{
	char	**envp;
	int		i;

	i = 0;
	envp = malloc(sizeof(char *) * (env_size(env) + 1));
	if (!envp)
		return (NULL);
	while (env)
	{
		if (env->raw)
		{
			envp[i] = ft_strdup(env->raw);
			if (!envp[i])
			{
				while (--i >= 0)
					free(envp[i]);
				return (free(envp), NULL);
			}
			i++;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

int	is_builtin(t_exec *cmd)
{
	if (!cmd || !cmd->cmd)
		return (0);
	if (ft_strcmp(cmd->cmd, "cd") == 0 || ft_strcmp(cmd->cmd, "echo") == 0
		|| ft_strcmp(cmd->cmd, "pwd") == 0 || ft_strcmp(cmd->cmd, "export") == 0
		|| ft_strcmp(cmd->cmd, "unset") == 0 || ft_strcmp(cmd->cmd, "env") == 0
		|| ft_strcmp(cmd->cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_exec *cmd, t_context *ctx, t_env *env)
{
	if (ft_strcmp(cmd->cmd, "cd") == 0)
		return (ft_cd(cmd, env));
	else if (ft_strcmp(cmd->cmd, "echo") == 0)
		return (ft_echo(cmd));
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		return (ft_export(env, cmd));
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		return (ft_unset(&env, cmd));
	else if (ft_strcmp(cmd->cmd, "env") == 0)
		return (ft_env(env));
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
		ft_exit(cmd, ctx);
	return (0);
}

int	create_pipe_if_needed(t_exec *cmd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return (-1);
		}
	}
	else
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
	}
	return (0);
}
