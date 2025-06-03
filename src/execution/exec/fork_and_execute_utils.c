/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_execute_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 19:17:19 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/03 17:04:26 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "errno.h"

void	handle_command_error(t_exec *cmd, t_resources *res, t_context *ctx)
{
	if (access(res->path, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		free_child(res, ctx);
		exit(127);
	}
	else if (access(res->path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		free_child(res, ctx);
		exit(126);
	}
}

void	execute_command(t_resources *res, t_context *ctx)
{
	execve(res->path, res->args, res->envp);
	perror("minishell: execve");
	free_child(res, ctx);
	exit(126);
}
