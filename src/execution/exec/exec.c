/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:10:27 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/25 11:29:37 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipe_if_needed(t_exec *cmd, int pipefd[2])
{
	if (cmd->next && pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

pid_t	fork_and_execute(t_exec *cmd, char **envp, int pipefd[2],
		int prev_pipe_end)
{
	pid_t	pid;
	char	**args;
	char	*path;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		setup_pipe_and_redir(cmd, pipefd, prev_pipe_end);
		args = args_to_array(cmd);
		if (!args)
			exit_with_error("args_to_array", NULL, NULL);
		path = get_cmd_path(envp, cmd->cmd);
		if (!path)
			exit_with_error("get_cmd_path", args, NULL);
		execve(path, args, envp);
		exit_with_error("execve", args, path);
	}
	return (pid);
}

void	close_unused_fd(t_exec *cmd, int pipefd[2], int *prev_pipe_end)
{
	if (*prev_pipe_end != -1)
		close(*prev_pipe_end);
	if (cmd->next)
	{
		*prev_pipe_end = pipefd[0];
		close(pipefd[1]);
	}
}

int	wait_for_childrens(pid_t last_pid)
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
	if (WIFSIGNALED(last_pid))
		return (128 + WTERMSIG(last_status));
	return (-1);
}

void	execute_pipeline(t_exec *cmd, char **envp)
{
	int		pipefd[2];
	int		prev_pipe_end;
	pid_t	pid;
	int		exit_code;

	prev_pipe_end = -1;
	while (cmd)
	{
		if (create_pipe_if_needed(cmd, pipefd) == -1)
			return ;
		pid = fork_and_execute(cmd, envp, pipefd, prev_pipe_end);
		if (pid == -1)
			return ;
		close_unused_fd(cmd, pipefd, &prev_pipe_end);
		cmd = cmd->next;
	}
	exit_code = wait_for_childrens(pid);
}

/*the exit code will be given to the global variable*/
