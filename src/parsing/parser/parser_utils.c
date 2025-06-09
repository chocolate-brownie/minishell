/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:15:37 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/09 22:48:17 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	validate_init_tokens(t_token *token_list, t_context *ctx)
{
	if (!ctx)
		return (-1);
	if (token_list == NULL || token_list->type == TOKEN_EOF)
		return (1);
	if (token_list->type == TOKEN_PIPE)
	{
		set_exit_code(ctx, ERR_SYNTAX, "|");
		return (-1);
	}
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
void	link_nodes(t_exec **head, t_exec **tail, t_exec *new_node)
{
	if (!head || !tail || !new_node)
		return ;
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
int	check_next_tok(t_token **curr_tok, t_context *ctx)
{
	if (!curr_tok || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), -1);
	if (*curr_tok == NULL || (*curr_tok)->type == TOKEN_EOF)
		return (0);
	if ((*curr_tok)->type == TOKEN_PIPE)
	{
		*curr_tok = (*curr_tok)->next;
		if (*curr_tok == NULL || (*curr_tok)->type == TOKEN_PIPE
			|| (*curr_tok)->type == TOKEN_EOF)
		{
			if (*curr_tok && ((*curr_tok)->type == TOKEN_PIPE
					|| (*curr_tok)->type == TOKEN_EOF))
				set_exit_code(ctx, ERR_SYNTAX, (*curr_tok)->value);
			else
				set_exit_code(ctx, ERR_SYNTAX, "|");
			return (-1);
		}
	}
	return (0);
}

void	invalseg_after_pipe(t_exec **headptr, t_exec **newptr,
		t_token **startptr, t_context *ctxptr)
{
	if (headptr && *headptr)
		free_exec_list(*headptr);
	if (newptr && *newptr)
	{
		free_single_exec_node_content(*newptr);
		free(*newptr);
	}
	if (startptr != NULL && *startptr != NULL)
		set_exit_code(ctxptr, ERR_SYNTAX, (*startptr)->value);
	else
		set_exit_code(ctxptr, ERR_SYNTAX, ERMSG_UNXTKN);
	return ;
}
