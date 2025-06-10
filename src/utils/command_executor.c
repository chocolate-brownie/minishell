/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 12:00:00 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/10 12:46:12 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_command_list(t_exec *exec_list, t_context *ctx)
{
	ctx->command_list = exec_list;
	if (DEBUG && exec_list)
	{
		printf("--- AST Structure (in execute_command_list) ---\n");
		print_exec_list(exec_list);
		printf("--- End AST Structure (in execute_command_list) ---\n");
	}
	if (!execute_pipeline(ctx))
	{
		cleanup_command_heredocs(exec_list, ctx);
		free_exec_list(exec_list);
		ctx->command_list = NULL;
		return (2);
	}
	cleanup_command_heredocs(exec_list, ctx);
	free_exec_list(exec_list);
	ctx->command_list = NULL;
	return (1);
}
