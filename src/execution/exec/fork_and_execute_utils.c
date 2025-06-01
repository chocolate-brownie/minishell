/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_execute_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:02:27 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/01 18:32:03 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	handle_command_not_found(t_exec *cmd, t_resources *res,
		t_context *ctx)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd->cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	free_child(res, ctx);
	exit(127);
}

void	execute_command(t_resources *res, t_context *ctx)
{
	execve(res->path, res->args, res->envp);
	perror("minishell: execve");
	free_child(res, ctx);
	exit(126);
}
