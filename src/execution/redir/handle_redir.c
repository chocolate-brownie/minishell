/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:51:55 by shasinan          #+#    #+#             */
/*   Updated: 2025/04/23 12:03:23 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	input_redir(t_redirs *redir)
{
	int	fd;

	fd = open(redir->path, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->path);
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup (input)");
		exit(1);
	}
	close(fd);
}

void	output_redir(t_redirs *redir)
{
	int	fd;

	fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(redir->path);
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup (output)");
		exit(1);
	}
	close(fd);
}

void	append_redir(t_redirs *redir)
{
	int	fd;

	fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->path);
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup (append)");
		exit(1);
	}
	close(fd);
}

void	handle_redir(t_exec *cmd)
{
	t_redirs	*redir;
	int			fd;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_INPUT)
			input_redir(redir);
		else if (redir->type == REDIR_OUTPUT)
			output_redir(redir);
		else if (redir->type == REDIR_APPEND)
			append_redir(redir);
		redir = redir->next;
	}
}
