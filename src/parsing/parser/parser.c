/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:50:18 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/05 21:27:12 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include <stdio.h>

static int	is_token(t_token *ptr)
{
	return (ptr->type == TOKEN_WORD || ptr->type == TOKEN_REDIR_IN
		|| ptr->type == TOKEN_REDIR_OUT || ptr->type == TOKEN_REDIR_APPEND
		|| ptr->type == TOKEN_REDIR_HEREDOC);
}

t_exec	*create_exec_node(t_token *curr_tok, t_exec *ptr)
{
	t_exec	*head;

	head = NULL;
	if (head == NULL && is_token(curr_tok))
	{
		ptr = malloc(sizeof(t_exec));
		if (!ptr)
			return (perror("malloc failed create_exec_node()"), NULL);
        if (curr_tok->type == TOKEN_WORD) {
            if (curr_tok->next->type == TOKEN_WORD)
                ptr->args = curr_tok->next->value; 
            ptr->cmd = curr_tok->value;
        }
		head = ptr;
	}
	return (head);
}

t_exec	*parser(t_token *token_list)
{
	t_token	*curr_tok;
	t_exec	*curr_cmd;
	t_exec	*head;

    curr_tok = token_list;
	set_null(2, (void **)curr_cmd, (void **)head);
	while (curr_tok && curr_tok->type != TOKEN_EOF)
	{
        head = create_exec_node(curr_tok, curr_cmd);
        if (!head)
            return (write(2, "Error: parser failed\n", 21), NULL);
	}
	return (head);
}
