/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:50:18 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/09 00:36:38 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	free_structs(char *errmsg, t_token *ptr_tkn, t_exec *ptr_exec)
{
	if (errmsg)
		ft_putstr_fd(errmsg, 2);
	if (ptr_tkn)
		free_token_list(ptr_tkn);
	if (ptr_exec)
		free_exec_list(ptr_exec);
	return ;
}

static int	validate_initial_tokens_and_handle_error(t_token *token_list)
{
	if (token_list == NULL || token_list->type == TOKEN_EOF)
		return (1);
	if (token_list->type == TOKEN_PIPE)
		return (free_structs(ERR_UNXP_TKN, token_list, NULL), -1);
	return (0);
}

/* If there are no nodes exists inside the t_exec struct what we have created
 * right now is the begining of the structure. if so the head_exec == new_exec
 * and tail_exec == new_exec
 * Otherwise it means we already have some commands in the list so
 * tail_exec->next is the new node we just ctreated
 *
 * HINT: I dont have to use the double pointer to the new_node since I am not
 * modfying the value but rather copying it */
static void	link_nodes(t_exec **head, t_exec **tail, t_exec *new_node)
{
	if (*head == NULL)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
	return ;
}

/* If the curr_tok (updated by the create_exec_node) is a PIPE advance the
 * pointer to the next token past the TOKEN_PIPE (this should activate the
 * while loop again )
 * if curr_tok == EOF, another PIPE or NULL its syntax error then free used
 * memory and return NULL */
static int	check_next_tok(t_token **curr_tok)
{
	if ((*curr_tok)->type == TOKEN_PIPE)
	{
		*curr_tok = (*curr_tok)->next;
		if (*curr_tok == NULL || (*curr_tok)->type == TOKEN_PIPE
			|| (*curr_tok)->type == TOKEN_EOF)
			return (-1);
	}
	return (0);
}

/* check whether the current token is EOF or if its NULL if it is return NULL
 * check whether the beginning of the command is  PIPE if it is free the
 * token_list and throw NULL
 * go through each token != NULL or EOF and process create the exec struct
 * reagarding to the corresponding token value after the process it should
 * return the pointer to the current token so I can check whether its pointing
 * to a PIPE at this point I have created a full t_execv */
t_exec	*parser(t_token *token_list)
{
	t_token	*curr_tok;
	t_exec	*head_exec;
	t_exec	*tail_exec;
	t_exec	*new_exec;

	if (validate_initial_tokens_and_handle_error(token_list) != 0)
		return (NULL);
	curr_tok = token_list;
	head_exec = NULL;
	tail_exec = NULL;
	while (curr_tok && curr_tok->type != TOKEN_EOF)
	{
		new_exec = create_exec_node(&curr_tok);
		if (new_exec == NULL)
			return (free_structs(ERR_CMD_SEG, token_list, head_exec), NULL);
		link_nodes(&head_exec, &tail_exec, new_exec);
		if (check_next_tok(&curr_tok) != 0)
			return (free_structs(ERR_UNXP_TKN, token_list, head_exec), NULL);
	}
	if (token_list != NULL)
		free_token_list(token_list);
	return (head_exec);
}
