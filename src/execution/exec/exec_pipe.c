/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 17:50:33 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/31 18:39:18 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	handle_early_exit_conditions(t_context *ctx,
		int *pipeline_exit_status)
{
	t_exec	*cmd;
	int		builtin_status;

	cmd = ctx->command_list;
	if (!cmd)
	{
		*pipeline_exit_status = 1;
		return (1);
	}
	builtin_status = execute_single_builtin(cmd, ctx);
	if (builtin_status >= 0)
	{
		ctx->last_exit_code = builtin_status;
		*pipeline_exit_status = 1;
		return (1);
	}
	else if (builtin_status == -2)
	{
		ctx->last_exit_code = 1;
		*pipeline_exit_status = 0;
		return (1);
	}
	return (0);
}

static pid_t	execute_pipeline_commands(t_context *ctx)
{
	t_exec	*cmd;
	int		pipefd[2];
	int		prev_read_end;
	pid_t	current_pid;
	pid_t	last_pid_in_pipeline;

	cmd = ctx->command_list;
	prev_read_end = -1;
	last_pid_in_pipeline = 0;
	while (cmd)
	{
		current_pid = cmd_loop(cmd, ctx, &prev_read_end, pipefd);
		if (!current_pid)
		{
			ctx->last_exit_code = 1;
			return (0);
		}
		last_pid_in_pipeline = current_pid;
		cmd = cmd->next;
	}
	return (last_pid_in_pipeline);
}

int	execute_pipeline(t_context *ctx)
{
	int		pipeline_early_exit_status;
	pid_t	last_executed_pid;

	if (handle_early_exit_conditions(ctx, &pipeline_early_exit_status))
	{
		return (pipeline_early_exit_status);
	}
	last_executed_pid = execute_pipeline_commands(ctx);
	if (last_executed_pid == 0)
	{
		return (0);
	}
	ctx->last_exit_code = wait_for_childrens(last_executed_pid);
	setup_signal_parent();
	return (1);
}
