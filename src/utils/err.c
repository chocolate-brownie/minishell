/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:28:55 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/08 19:54:25 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_state(int argc, char *argv[])
{
	if (argc > 1 || !argv[0])
	{
		perror("The program accepts no arguments\n");
		return (1);
	}
	if (clear_term() != 0)
		return (1);
	return (0);
}

void	free_structs(char *errmsg, t_token *ptr_tkn, t_exec *ptr_exec)
{
	ft_putstr_fd(errmsg, 2);
	if (ptr_tkn != NULL)
	{
		free_token_list(ptr_tkn);
		ft_putstr_fd("t_token freed\n", 2);
	}
	if (ptr_exec != NULL)
	{
		free_exec_list(ptr_exec);
		ft_putstr_fd("t_exec freed\n", 2);
	}
}
