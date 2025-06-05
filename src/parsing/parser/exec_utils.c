/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 00:07:08 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/19 15:26:07 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_args	*create_arg_node(char *value, t_context *ctx)
{
	t_args	*new_node;

	new_node = (t_args *)malloc(sizeof(t_args));
	if (!new_node)
	{
		set_exit_code(ctx, ERR_MALLOC, NULL);
		return (NULL);
	}
	new_node->value = ft_strdup(value);
	if (!new_node->value)
	{
		free(new_node);
		set_exit_code(ctx, ERR_MALLOC, NULL);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

t_args	*append_arg(t_args **args_list_head, char *value, t_context *ctx)
{
	t_args	*new_arg_node;
	t_args	*current;

	if (!value || !ctx)
		return (*args_list_head);
	new_arg_node = create_arg_node(value, ctx);
	if (!new_arg_node)
		return (NULL);
	if (*args_list_head == NULL)
		*args_list_head = new_arg_node;
	else
	{
		current = *args_list_head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_arg_node;
	}
	return (*args_list_head);
}

int	process_word_token(t_exec *exec_node, t_token **curr, t_context *ctx)
{
	if (!exec_node || !curr || !*curr || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), -1);
	if (exec_node->cmd == NULL)
	{
		if ((*curr)->value && (*curr)->value[0] == '\0')
		{
			*curr = (*curr)->next;
			return (0);
		}
		exec_node->cmd = ft_strdup((*curr)->value);
		if (!exec_node->cmd)
			return (set_exit_code(ctx, ERR_MALLOC, NULL), -1);
	}
	else
	{
		if (!append_arg(&(exec_node->args), (*curr)->value, ctx))
			return (-1);
	}
	*curr = (*curr)->next;
	return (0);
}
