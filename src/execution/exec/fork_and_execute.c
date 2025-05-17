/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:13:56 by shasinan          #+#    #+#             */
/*   Updated: 2025/05/17 10:37:54 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_with_error(char **args, char *path, char **envp, int type)
{
	if (type == 1)
		exit(1);
	if (type == 2)
	{
		free_tab(args);
		exit(1);
	}
	if (type == 3)
	{
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(" : command not found\n", 2);
		free_tab(args);
		free_tab(envp);
		free(path);
		exit(127);
	}
	if (type == 4)
	{
		perror("execve");
		free_tab(args);
		free(path);
		free_tab(envp);
		exit(126);
	}
}

static void	setup_pipe_and_redir(t_exec *cmd, int pipefd[2], int prev_pipe_end)
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
		handle_redir(cmd);
}

static void	child_process(t_context *ctx, t_exec *cmd, int pipefd[2],
		int prev_pipe_end)
{
	char	**args;
	char	*path;
	char	**envp;

	setup_pipe_and_redir(cmd, pipefd, prev_pipe_end);
	if (is_builtin(cmd))
	{
		ctx->last_exit_code = execute_builtin(cmd, ctx, ctx->envp);
		free_env(ctx->envp);
		exit(ctx->last_exit_code);
	}
	args = args_to_array(cmd, 1);
	if (!args)
		exit_with_error(args, NULL, NULL, 1);
	envp = env_to_envp(ctx->envp);
	if (!envp)
		exit_with_error(args, NULL, envp, 2);
	if (ft_strchr(cmd->cmd, '/'))
		path = ft_strdup(cmd->cmd);
	else
		path = get_cmd_path(envp, cmd->cmd);
	if (!path || access(path, X_OK) != 0)
		exit_with_error(args, path, envp, 3);
	execve(path, args, envp);
	exit_with_error(args, path, envp, 4);
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
		child_process(ctx, cmd, pipefd, prev_read_end);
	return (pid);
}
