/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:13:56 by shasinan          #+#    #+#             */
/*   Updated: 2025/06/03 12:21:58 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	print_signal_msg(int status, int *message_printed)
{
	int	sig;

	sig = WTERMSIG(status);
	if (sig == SIGQUIT)
		write(STDERR_FILENO, "Quit (core dumped)\n", 20);
	else if (sig == SIGINT)
		write(STDERR_FILENO, "\n", 1);
	*message_printed = 1;
}

static int	setup_pipe_and_redir(t_exec *cmd, int pipefd[2], int prev_pipe_end,
		t_context *ctx)
{
	if (prev_pipe_end != -1)
	{
		if (dup2(prev_pipe_end, STDIN_FILENO) == -1)
			return (perror("dup2 (prev_pipe_end to STDIN_FILENO)"), 0);
		close(prev_pipe_end);
	}
	if (cmd->next)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			return (perror("dup2 (pipefd[1] to STDOUT_FILENO)"), 0);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	if (!handle_redir(cmd, ctx))
		return (0);
	return (1);
}

static void	child_process(t_context *ctx, t_exec *cmd, int pipefd[2],
		int prev_pipe_end)
{
	int	exit_code;

	if (!setup_pipe_and_redir(cmd, pipefd, prev_pipe_end, ctx))
	{
		free_all(ctx);
		exit(1);
	}
	if (is_builtin(cmd))
	{
		exit_code = execute_builtin(cmd, ctx, ctx->envp);
		free_all(ctx);
		exit(exit_code);
	}
	child_execute_external_command(ctx, cmd);
}

pid_t	fork_and_execute(t_context *ctx, t_exec *cmd, int pipefd[2],
		int prev_read_end)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	if (pid == 0)
	{
		setup_signal_child();
		child_process(ctx, cmd, pipefd, prev_read_end);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	return (pid);
}
