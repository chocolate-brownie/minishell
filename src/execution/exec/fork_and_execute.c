/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:13:56 by shasinan          #+#    #+#             */
/*   Updated: 2025/06/01 17:02:41 by mgodawat         ###   ########.fr       */
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

static void	child_process(t_context *ctx, t_exec *cmd, int pipefd[2],
		int prev_pipe_end)
{
	t_resources	res;

	res.args = NULL;
	res.envp = NULL;
	res.path = NULL;
	if (!setup_pipe_and_redir(cmd, pipefd, prev_pipe_end, ctx))
	{
		free_all(ctx);
		exit(1);
	}
	if (is_builtin(cmd))
	{
		ctx->last_exit_code = execute_builtin(cmd, ctx, ctx->envp);
		free_env(ctx->envp);
		exit(ctx->last_exit_code);
	}
	if (!prepare_exec_resources(cmd, ctx, &res))
	{
		free_all(ctx);
		exit(1);
	}
	if (access(res.path, X_OK) != 0)
		handle_command_not_found(cmd, &res, ctx);
	execute_command(&res, ctx);
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
