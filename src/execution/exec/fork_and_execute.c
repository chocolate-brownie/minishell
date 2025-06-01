/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:13:56 by shasinan          #+#    #+#             */
/*   Updated: 2025/06/01 14:32:29 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	free_and_exit(t_resources *res, int type, t_context *ctx)
{
	if (type == 1)
		exit(1);
	if (type == 2)
	{
		free_child(res, ctx);
		exit(1);
	}
	if (type == 3)
	{
		ft_putstr_fd(res->args[0], 2);
		ft_putstr_fd(" : command not found\n", 2);
		free_child(res, ctx);
		exit(127);
	}
	if (type == 4)
	{
		perror("execve");
		free_child(res, ctx);
		exit(126);
	}
}

static void	setup_pipe_and_redir(t_exec *cmd, int pipefd[2], int prev_pipe_end,
		t_context *ctx)
{
	if (prev_pipe_end != -1)
	{
		if (dup2(prev_pipe_end, STDIN_FILENO) == -1)
		{
			perror("dup (pipe_prev_end)");
			exit(1);
		}
		close(prev_pipe_end);
	}
	if (cmd->next)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup (pipefd[1])");
			exit(1);
		}
		close(pipefd[1]);
		close(pipefd[0]);
	}
	if (cmd->redirs)
		handle_redir(cmd, ctx);
}

static void	child_process(t_context *ctx, t_exec *cmd, int pipefd[2],
		int prev_pipe_end)
{
	t_resources	res;

	setup_pipe_and_redir(cmd, pipefd, prev_pipe_end, ctx);
	if (is_builtin(cmd))
	{
		ctx->last_exit_code = execute_builtin(cmd, ctx, ctx->envp);
		free_env(ctx->envp);
		exit(ctx->last_exit_code);
	}
	res.args = args_to_array(cmd, 1);
	if (!res.args)
		free_and_exit(&res, 1, ctx);
	res.envp = env_to_envp(ctx->envp);
	if (!res.envp)
		free_and_exit(&res, 2, ctx);
	if (ft_strchr(cmd->cmd, '/'))
		res.path = ft_strdup(cmd->cmd);
	else
		res.path = get_cmd_path(res.envp, cmd->cmd);
	if (!res.path || access(res.path, X_OK) != 0)
		free_and_exit(&res, 3, ctx);
	execve(res.path, res.args, res.envp);
	free_and_exit(&res, 4, ctx);
}

pid_t	fork_and_execute(t_context *ctx, t_exec *cmd, int pipefd[2],
		int prev_read_end)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
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
