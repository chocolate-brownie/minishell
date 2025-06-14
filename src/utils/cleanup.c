/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 21:07:50 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/09 18:43:03 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 TODO: free(ctx->pids); free pids array
*/
void	cleanup_tcontext(t_context *ctx)
{
	if (!ctx)
		return ;
	if (ctx->envp)
		free_env_list(ctx->envp);
	if (ctx->command_list)
		free_exec_list(ctx->command_list);
	if (ctx->pids)
		free(ctx->pids);
	if (ctx->stdin_backup != -1)
		close(ctx->stdin_backup);
	if (ctx->stdout_backup != -1)
		close(ctx->stdout_backup);
	cleanup_all_active_heredocs(ctx);
}

void	cleanup_resources(char *cmd, t_token *token_list, t_context *ctx)
{
	if (cmd)
	{
		free(cmd);
		cmd = NULL;
	}
	if (token_list)
	{
		free_token_list(token_list);
		token_list = NULL;
	}
	if (ctx)
	{
		cleanup_tcontext(ctx);
		ctx = NULL;
	}
}
