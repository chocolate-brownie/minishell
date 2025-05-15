/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:28:55 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/11 22:30:44 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_state(int argc, char *argv[])
{
	if (argc > 1 || !argv[0])
	{
		set_exit_code(NULL, ERR_INVALID_INPUT, ERMSG_INVALIDARG);
		return (1);
	}
	return (0);
}

void	free_structs(char *errmsg, t_token *ptr_tkn, t_exec *ptr_exec)
{
	if (errmsg)
		write(2, errmsg, ft_strlen(errmsg));
	if (ptr_tkn != NULL)
		free_token_list(ptr_tkn);
	if (ptr_exec != NULL)
		free_exec_list(ptr_exec);
}
