/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:13:56 by shasinan          #+#    #+#             */
/*   Updated: 2025/04/24 17:17:52 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**args_to_array(t_exec *cmd)
{
	char	**arr;
	int		len;
	int		i;
	t_args	*a;

	a = cmd->args;
	len = 0;
	while (a)
	{
		len++;
		a = a->next;
	}
	arr = malloc(sizeof(char *) * (len + 2));
	if (!arr)
		return (NULL);
	i = 0;
	arr[i++] = cmd->cmd;
	a = cmd->args;
	while (a)
	{
		arr[i++] = a->value;
		a = a->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	exit_with_error(char *msg, char **args, char *path)
{
	perror(msg);
	if (args)
		free(args);
	if (path)
		free(path);
	exit(1);
}

void	setup_pipe_and_redir(t_exec *cmd, int pipefd[2], int prev_pipe_end)
{
	if (prev_pipe_end != -1)
	{
		dup2(prev_pipe_end, STDIN_FILENO);
		close(prev_pipe_end);
	}
	if (cmd->next)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	if (cmd->redirs)
		handle_redir(cmd);
}
