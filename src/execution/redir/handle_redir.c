/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:51:55 by shasinan          #+#    #+#             */
/*   Updated: 2025/05/26 15:49:16 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redir_input(t_redirs *redir)
{
	int	fd;

	fd = open(redir->path, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->path);
		return (0);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup (input)");
		return (0);
	}
	close(fd);
	return (1);
}

static int	redir_output(t_redirs *redir)
{
	int	fd;

	fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(redir->path);
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup (output)");
		return (0);
	}
	close(fd);
	return (1);
}

static int	redir_append(t_redirs *redir)
{
	int	fd;

	fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->path);
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup (append)");
		return (0);
	}
	close(fd);
	return (1);
}

int	handle_redir(t_exec *cmd, t_context *ctx)
{
	t_redirs	*redir;

	redir = cmd->redirs;
	if (!redir)
		return (1);
	ctx->stdin_backup = dup(STDIN_FILENO);
	ctx->stdout_backup = dup(STDOUT_FILENO);
	if (ctx->stdin_backup == -1 || ctx->stdout_backup == -1)
		return (0);
	while (redir)
	{
		if (redir->type == REDIR_INPUT && !redir_input(redir))
			return (0);
		else if (redir->type == REDIR_OUTPUT && !redir_output(redir))
			return (0);
		else if (redir->type == REDIR_APPEND && !redir_append(redir))
			return (0);
		redir = redir->next;
	}
	return (1);
}

int	restore_stdio(t_context *ctx)
{
	if (dup2(ctx->stdin_backup, STDIN_FILENO) == -1)
	{
		perror("dup stdin_backup");
		return (0);
	}
	if (dup2(ctx->stdout_backup, STDOUT_FILENO) == -1)
	{
		perror("dup stdout_backup");
		return (0);
	}
	close (ctx->stdout_backup);
	close (ctx->stdin_backup);
	return (1);
}
