/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:11:08 by shasinan          #+#    #+#             */
/*   Updated: 2025/05/19 12:31:06 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(t_context *ctx, t_env *env)
{
	if (env)
		free_env(env);
	if (ctx->command_list)
		free_exec_list(ctx->command_list);
}
