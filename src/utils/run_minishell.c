/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_minishell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 00:35:07 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/11 21:49:24 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 TODO: free_env_list(ctx->envp); Free environment variables
 TODO: free_exec_list(ctx->command_list); free command list
 TODO: free(ctx->pids); free pids array
*/
static void	cleanup_tcontext(t_context *ctx)
{
	if (!ctx)
		return ;
	if (ctx->stdin_backup != -1)
		close(ctx->stdin_backup);
	if (ctx->stdout_backup != -1)
		close(ctx->stdout_backup);
	free(ctx);
}

static int	process_command(char *cmd, t_token **token_list, t_context *ctx)
{
	t_exec	*exec_list;

	if (!cmd || !token_list || !ctx)
		return (-1);
	*token_list = lexer(cmd, ctx);
	if (*token_list == NULL)
		return (free(cmd), 0);
	exec_list = parser(*token_list, ctx);
	if (!exec_list)
	{
		free_token_list(*token_list);
		*token_list = NULL;
		return (free(cmd), 0);
	}
	free_exec_list(exec_list);
	return (1);
}

static void	cleanup_resources(char *cmd, t_token *token_list, t_context *ctx)
{
	if (cmd)
		free(cmd);
	if (token_list)
		free_token_list(token_list);
	if (ctx)
		cleanup_tcontext(ctx);
}

int	run_minishell(char *cmd, t_token *token_list, t_context *ctx)
{
	int	result;

	if (!ctx)
		return (1);
	while (1)
	{
		cmd = read_cmd(ctx);
		result = process_command(cmd, &token_list, ctx);
		if (result == -1)
			break ;
		if (result == 0)
			continue ;
		if (ctx->should_exit != 0)
			break ;
	}
	cleanup_resources(cmd, token_list, ctx);
	return (ctx->last_exit_code);
}
