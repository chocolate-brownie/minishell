/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:10:27 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/19 13:03:59 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_unused_fd(t_exec *cmd, int pipefd[2], int *prev_read_end)
{
	if (*prev_read_end != -1)
		close(*prev_read_end);
	if (cmd->next)
	{
		*prev_read_end = pipefd[0];
		close(pipefd[1]);
	}
	else
	{
		if (pipefd[0] > 2)
			close(pipefd[0]);
		if (pipefd[1] > 2)
			close(pipefd[1]);
	}
}

static int	wait_for_childrens(pid_t last_pid)
{
	pid_t	pid;
	int		status;
	int		last_status;

	last_status = 0;
	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
			last_status = status;
		pid = wait(&status);
	}
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	if (WIFSIGNALED(last_status))
		return (128 + WTERMSIG(last_status));
	return (-1);
}

pid_t	cmd_loop(t_exec *cmd, t_context *ctx, int *prev_read_end, int pipefd[2])
{
	pid_t	pid;

	if (create_pipe_if_needed(cmd, pipefd) == -1)
		return (0);
	pid = fork_and_execute(ctx, cmd, pipefd, *prev_read_end);
	if (pid == -1)
		return (0);
	close_unused_fd(cmd, pipefd, prev_read_end);
	return (pid);
}

int	execute_single_builtin(t_exec *cmd, t_context *ctx)
{
	int	exit_code;

	if (is_builtin(cmd) && !cmd->next)
	{
		if (!handle_redir(cmd, ctx))
			return (1);
		exit_code = execute_builtin(cmd, ctx, ctx->envp);
		restore_stdio(ctx);
		return (exit_code);
	}
	return (-1);
}

void	execute_pipeline(t_context *ctx)
{
	t_exec	*cmd;
	int		pipefd[2];
	int		prev_read_end;
	pid_t	pid;
	int		builtin_status;

	cmd = ctx->command_list;
	builtin_status = execute_single_builtin(cmd, ctx);
	if (builtin_status != -1)
	{
		ctx->last_exit_code = builtin_status;
		return ;
	}
	prev_read_end = -1;
	while (cmd)
	{
		pid = cmd_loop(cmd, ctx, &prev_read_end, pipefd);
		if (!pid)
		{
			ctx->last_exit_code = 1;
			return ;
		}
		cmd = cmd->next;
	}
	ctx->last_exit_code = wait_for_childrens(pid);
}

/*-builtins must return the exit code
	-if something wrong, maybe free all the resources*/

// exit
//-if something wrong, maybe free all the resources
// pids
// free : (env), etc
// heredoc
// signaal

//$?
// exit is builtin