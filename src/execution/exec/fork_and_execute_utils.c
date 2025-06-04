/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_execute_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 19:17:19 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/04 09:36:49 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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
	struct stat	st;

	if (stat(res->path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(res->path, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		free_child(res, ctx);
		exit(126);
	}
	execve(res->path, res->args, res->envp);
	perror("minishell: execve");
	free_child(res, ctx);
	exit(126);
}
