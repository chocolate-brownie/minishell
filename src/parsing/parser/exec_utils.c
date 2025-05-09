/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 00:07:08 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/09 00:27:49 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_exec	*init_exec_node(void)
{
	t_exec	*new_node;

	new_node = NULL;
	new_node = malloc(sizeof(t_exec));
	if (!new_node)
		return (write(2, "Error: malloc init_exec_node()\n", 31), NULL);
	new_node->cmd = NULL;
	new_node->args = NULL;
	new_node->redirs = NULL;
	new_node->next = NULL;
	return (new_node);
}

static t_args	*append_arg(t_args **args_list_head, char *value)
{
	t_args	*new_arg_node;
	t_args	*current;

	if (!value)
		return (*args_list_head);
	new_arg_node = (t_args *)malloc(sizeof(t_args));
	if (!new_arg_node)
		return (perror("minishell: malloc error in append_arg"), NULL);
	new_arg_node->value = ft_strdup(value);
	if (!new_arg_node->value)
	{
		perror("minishell: ft_strdup error in append_arg");
		return (free(new_arg_node), NULL);
	}
	new_arg_node->next = NULL;
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

static t_redirs	*append_redir(t_redirs **redir_list_head, t_redir_type type,
		char *path)
{
	t_redirs	*new_redir_node;
	t_redirs	*current;

	if (!path)
		return (*redir_list_head);
	new_redir_node = (t_redirs *)malloc(sizeof(t_redirs));
	if (!new_redir_node)
		return (perror("minishell: malloc error in append_redir"), NULL);
	new_redir_node->path = ft_strdup(path);
	if (!new_redir_node->path)
		return (free(new_redir_node), NULL);
	new_redir_node->type = type;
	new_redir_node->next = NULL;
	if (*redir_list_head == NULL)
		*redir_list_head = new_redir_node;
	else
	{
		current = *redir_list_head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_redir_node;
	}
	return (*redir_list_head);
}

int	process_word_token(t_exec *exec_node, t_token **curr_token_ref)
{
	if (exec_node->cmd == NULL)
	{
		exec_node->cmd = ft_strdup((*curr_token_ref)->value);
		if (!exec_node->cmd)
			return (perror("minishell: ft_strdup error for command"), -1);
	}
	else
	{
		if (!append_arg(&(exec_node->args), (*curr_token_ref)->value))
			return (perror("minishell: append_arg error"), -1);
	}
	*curr_token_ref = (*curr_token_ref)->next;
	return (0);
}

int	process_redir_token(t_exec *exec_node, t_token **curr_token_ref,
		t_redir_type redir_type)
{
	t_token	*operator_token;
	t_token	*file_token;

	operator_token = *curr_token_ref;
	file_token = operator_token->next;
	if (file_token == NULL || file_token->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		if (file_token && file_token->value != NULL)
		{
			ft_putstr_fd(file_token->value, 2);
			ft_putstr_fd("'", 2);
		}
		else if (file_token != NULL)
			ft_putstr_fd("(invalid redirection target)", 2);
		else
			ft_putstr_fd("newline", 2);
		ft_putstr_fd("\n", 2);
		return (-1);
	}
	if (!append_redir(&(exec_node->redirs), redir_type, file_token->value))
		return (perror("minishell: append_redir error"), -1);
	*curr_token_ref = file_token->next;
	return (0);
}
