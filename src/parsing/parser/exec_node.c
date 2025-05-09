/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:55:15 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/09 00:26:38 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static t_exec	*fill_exec_node_from_tokens(t_exec *node, t_token **token_ptr)
{
	t_redir_type	redir_type;
	t_redir_type	invalid_redir;

	invalid_redir = (t_redir_type)-1;
	while (*token_ptr && (*token_ptr)->type != TOKEN_EOF
		&& (*token_ptr)->type != TOKEN_PIPE)
	{
		if ((*token_ptr)->type == TOKEN_WORD)
		{
			if (process_word_token(node, token_ptr) != 0)
				return (token_failure(node), NULL);
		}
		else
		{
			redir_type = get_redir_type((*token_ptr)->type);
			if (redir_type != invalid_redir)
			{
				if (process_redir_token(node, token_ptr, redir_type) != 0)
					return (token_failure(node), NULL);
			}
			else
				return (unexpected_token(node, *token_ptr), NULL);
		}
	}
	return (node);
}

t_exec	*create_exec_node(t_token **token_ptr)
{
	t_exec	*new_node;

	if (!token_ptr || !*token_ptr)
		return (NULL);
	new_node = init_exec_node();
	if (!new_node)
		return (ft_putstr_fd("init_exec_node failed", 2), NULL);
	new_node = fill_exec_node_from_tokens(new_node, token_ptr);
	return (new_node);
}
