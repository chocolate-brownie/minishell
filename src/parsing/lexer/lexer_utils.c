/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:02:06 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/06 13:06:58 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	validate_input(const char *cmd, t_context *ctx)
{
	if (!cmd)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), 0);
	if (!ctx)
		return (0);
	if (cmd[0] == '\0')
		return (set_exit_code(ctx, 0, NULL), 0);
	return (1);
}

void	init_token_list(t_token **list_head, t_token **list_tail)
{
	*list_head = NULL;
	*list_tail = NULL;
}
