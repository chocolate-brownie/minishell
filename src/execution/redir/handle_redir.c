/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:51:55 by shasinan          #+#    #+#             */
/*   Updated: 2025/06/07 19:45:07 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	backup_stdio(t_context *ctx)
{
	ctx->stdin_backup = dup(STDIN_FILENO);
	ctx->stdout_backup = dup(STDOUT_FILENO);
	if (ctx->stdin_backup == -1 || ctx->stdout_backup == -1)
	{
		perror("minishell: dup (for stdio backup)");
		if (ctx->stdin_backup != -1)
			close(ctx->stdin_backup);
		if (ctx->stdout_backup != -1)
			close(ctx->stdout_backup);
		ctx->stdin_backup = -1;
		ctx->stdout_backup = -1;
		return (0);
	}
	return (1);
}

static int	apply_redirection(t_redirs *redir, t_context *ctx)
{
	if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
	{
		if (!redir_input(redir))
		{
			restore_stdio(ctx);
			return (0);
		}
	}
	else if (redir->type == REDIR_OUTPUT)
	{
		if (!redir_output(redir))
		{
			restore_stdio(ctx);
			return (0);
		}
	}
	else if (redir->type == REDIR_APPEND)
	{
		if (!redir_append(redir))
		{
			restore_stdio(ctx);
			return (0);
		}
	}
	return (1);
}

int	handle_redir(t_exec *cmd, t_context *ctx)
{
	t_redirs	*current_redir;

	if (!cmd || !ctx)
		return (0);
	current_redir = cmd->redirs;
	if (!current_redir)
		return (1);
	if (!backup_stdio(ctx))
		return (0);
	while (current_redir)
	{
		if (!apply_redirection(current_redir, ctx))
			return (0);
		current_redir = current_redir->next;
	}
	return (1);
}

int	restore_stdio(t_context *ctx)
{
	if (ctx->stdin_backup != -1)
	{
		if (dup2(ctx->stdin_backup, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 (restore STDIN_FILENO)");
			return (0);
		}
		close(ctx->stdin_backup);
		ctx->stdin_backup = -1;
	}
	if (ctx->stdout_backup != -1)
	{
		if (dup2(ctx->stdout_backup, STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 (restore STDOUT_FILENO)");
			return (0);
		}
		close(ctx->stdout_backup);
		ctx->stdout_backup = -1;
	}
	return (1);
}
