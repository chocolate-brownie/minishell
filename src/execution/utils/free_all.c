/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:11:08 by shasinan          #+#    #+#             */
/*   Updated: 2025/06/09 18:48:32 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	free_all(t_context *ctx)
{
	if (!ctx)
		return ;
	if (ctx->token_list)
	{
		free_token_list(ctx->token_list);
		ctx->token_list = NULL;
	}
	if (ctx->envp)
	{
		free_env(ctx->envp);
		ctx->envp = NULL;
	}
	if (ctx->command_list)
	{
		free_exec_list(ctx->command_list);
		ctx->command_list = NULL;
	}
	free(ctx);
}

void	free_child(t_resources *res, t_context *ctx)
{
	if (res->args)
		free_tab(res->args);
	if (res->path)
		free(res->path);
	if (res->envp)
		free_tab(res->envp);
	free_all(ctx);
}
