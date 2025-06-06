/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:00:00 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/06 13:07:52 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	prepare_exec_resources(t_exec *cmd, t_context *ctx,
		t_resources *res)
{
	res->args = args_to_array(cmd, 1);
	if (!res->args)
		return (0);
	res->envp = env_to_envp(ctx->envp);
	if (!res->envp)
	{
		free_tab(res->args);
		res->args = NULL;
		return (0);
	}
	if (ft_strchr(cmd->cmd, '/'))
		res->path = ft_strdup(cmd->cmd);
	else
		res->path = get_cmd_path(res->envp, cmd->cmd);
	if (!res->path)
	{
		free_tab(res->args);
		res->args = NULL;
		free_tab(res->envp);
		res->envp = NULL;
		return (0);
	}
	return (1);
}

static void	error_message(t_context *ctx, t_exec *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd->cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	free_all(ctx);
	exit(127);
}

/*
** Prepares resources and executes an external command in the child process.
** This function is called after built-in checks and will exit the child process.
*/
void	child_execute_external_command(t_context *ctx, t_exec *cmd)
{
	t_resources	res;

	if (!cmd->cmd || !cmd->cmd[0])
	{
		free_all(ctx);
		exit(0);
	}
	res.args = NULL;
	res.envp = NULL;
	res.path = NULL;
	if (!prepare_exec_resources(cmd, ctx, &res))
	{
		if (!res.path && cmd && cmd->cmd && !ft_strchr(cmd->cmd, '/'))
			error_message(ctx, cmd);
		free_all(ctx);
		exit(1);
	}
	else if (ft_strchr(cmd->cmd, '/'))
	{
		if (access(res.path, X_OK) != 0)
			handle_command_error(cmd, &res, ctx);
	}
	execute_command(&res, ctx);
}
