/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 00:07:08 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/11 22:23:31 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static t_args	*create_arg_node(char *value, t_context *ctx)
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

static t_args	*append_arg(t_args **args_list_head, char *value,
		t_context *ctx)
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

int	process_redir_token(t_exec *exec_node, t_token **curr,
		t_redir_type redir_type, t_context *ctx)
{
	t_token	*operator_token;
	t_token	*file_token;

	if (!exec_node || !curr || !*curr || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), -1);
	operator_token = *curr;
	file_token = operator_token->next;
	if (file_token == NULL || file_token->type != TOKEN_WORD)
	{
		if (file_token)
			set_exit_code(ctx, ERR_SYNTAX, file_token->value);
		else
			set_exit_code(ctx, ERR_SYNTAX, "newline");
		return (-1);
	}
	if (!append_redir(&(exec_node->redirs), redir_type, file_token->value, ctx))
		return (-1);
	*curr = file_token->next;
	return (0);
}
