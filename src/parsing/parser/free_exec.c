/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 15:24:45 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/09 03:12:17 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/exec.h"
#include "../../../includes/minishell.h"

static void	free_args_list(t_args *args)
{
	t_args	*curr;
	t_args	*next;

	curr = args;
	while (curr != NULL)
	{
		next = curr->next;
		if (curr->value != NULL)
		{
			free(curr->value);
			curr->value = NULL;
		}
		free(curr);
		curr = next;
	}
}

static void	free_redirs_list(t_redirs *redirs)
{
	t_redirs	*curr;
	t_redirs	*next;

	curr = redirs;
	while (curr)
	{
		next = curr->next;
		if (curr->path)
		{
			free(curr->path);
			curr->path = NULL;
		}
		free(curr);
		curr = next;
	}
}

void	free_single_exec_node_content(t_exec *node)
{
	if (node == NULL)
		return ;
	if (node->cmd != NULL)
	{
		free(node->cmd);
		node->cmd = NULL;
	}
	if (node->args != NULL)
	{
		free_args_list(node->args);
		node->args = NULL;
	}
	if (node->redirs != NULL)
	{
		free_redirs_list(node->redirs);
		node->redirs = NULL;
	}
}

void	free_exec_list(t_exec *head)
{
	t_exec	*current;
	t_exec	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free_single_exec_node_content(current);
		free(current);
		current = next;
	}
}
