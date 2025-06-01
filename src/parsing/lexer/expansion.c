/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:00:00 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/01 21:07:44 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*
** Appends the string representation of the last exit code to `acc_val`.
** Advances `*cmd_idx_ptr` past "$?".
** Returns the new accumulated string, or NULL on malloc failure.
** `acc_val` is freed by this function if a new string is returned.
*/
char	*expand_dollar_question(char *acc_val, int *cmd_idx_ptr, t_context *ctx)
{
	char	*exit_code_str;
	char	*new_acc_val;

	if (!cmd_idx_ptr || !ctx)
	{
		if (ctx)
			set_exit_code(ctx, ERR_INVALID_INPUT,
				"expand_dollar_question args");
		return (acc_val);
	}
	exit_code_str = ft_itoa(ctx->last_exit_code);
	if (!exit_code_str)
	{
		set_exit_code(ctx, ERR_MALLOC, "itoa in expand_dollar_question");
		return (NULL);
	}
	new_acc_val = append_extracted(acc_val, exit_code_str, ctx);
	if (!new_acc_val)
	{
		return (NULL);
	}
	*cmd_idx_ptr += 2;
	return (new_acc_val);
}
