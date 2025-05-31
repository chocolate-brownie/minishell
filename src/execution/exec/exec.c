/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:10:27 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/31 17:53:54 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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

int	wait_for_childrens(pid_t last_pid)
{
	pid_t	pid;
	int		status;
	int		last_status;
	int		message_printed;

	message_printed = 0;
	last_status = 0;
	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
			last_status = status;
		pid = wait(&status);
		if (pid == last_pid && WIFSIGNALED(status) && !message_printed)
			print_signal_msg(status, &message_printed);
	}
	if (!message_printed && WIFSIGNALED(last_status))
		print_signal_msg(last_status, &message_printed);
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	if (WIFSIGNALED(last_status))
		return (128 + WTERMSIG(last_status));
	return (-1);
}

pid_t	cmd_loop(t_exec *cmd, t_context *ctx, int *prev_read_end,
		int pipefd[2])
{
	pid_t	pid;

	if (create_pipe_if_needed(cmd, pipefd) == -1)
	{
		return (0);
	}
	pid = fork_and_execute(ctx, cmd, pipefd, *prev_read_end);
	if (pid == -1)
	{
		return (0);
	}
	close_unused_fd(cmd, pipefd, prev_read_end);
	return (pid);
}

int	execute_single_builtin(t_exec *cmd, t_context *ctx)
{
	int	exit_code;

	if (is_builtin(cmd) && !cmd->next)
	{
		if (!handle_redir(cmd, ctx))
		{
			return (-2);
		}
		exit_code = execute_builtin(cmd, ctx, ctx->envp);
		if (!restore_stdio(ctx))
		{
			return (-2);
		}
		return (exit_code);
	}
	return (-1);
}
