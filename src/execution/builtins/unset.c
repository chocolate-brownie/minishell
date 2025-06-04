/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:41:39 by shasinan          #+#    #+#             */
/*   Updated: 2025/06/04 17:47:08 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
All static helper functions (get_id, free_node, unset_var,
is_valid_unset_identifier, process_unset_argument)
have been moved to unset_utils.c
*/

int	ft_unset(t_env **env, t_exec *cmd)
{
	char	**args;
	int		i;
	int		malloc_error_flag;
	int		return_status;

	args = args_to_array(cmd, 0);
	if (!args)
		return (1);
	i = 0;
	malloc_error_flag = 0;
	return_status = 0;
	while (args[i])
	{
		process_unset_argument(args[i], env, &return_status,
			&malloc_error_flag);
		i++;
	}
	free_tab(args);
	if (malloc_error_flag)
		return (1);
	return (return_status);
}
