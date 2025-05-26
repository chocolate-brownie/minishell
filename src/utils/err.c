/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:28:55 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/27 01:37:04 by mgodawat         ###   ########.fr       */
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

void	free_env_list(t_env *env_list_head)
{
	t_env	*current;
	t_env	*next_node;

	current = env_list_head;
	while (current != NULL)
	{
		next_node = current->next;
		if (current->id)
			free(current->id);
		if (current->value)
			free(current->value);
		if (current->raw)
			free(current->raw);
		free(current);
		current = next_node;
	}
}

int	cleanup_after_sigint(char *cmd, t_token **token_list)
{
	if (*token_list)
		return (free_token_list(*token_list), *token_list = NULL, free(cmd),
			-2);
	return (free(cmd), -2);
}

int	cleanup_failed_exec(char *cmd, t_token **token_list)
{
	return (free_token_list(*token_list), *token_list = NULL, free(cmd), 0);
}
