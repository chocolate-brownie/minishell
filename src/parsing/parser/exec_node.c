/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:55:15 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/09 22:35:31 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	handle_redirection(t_exec *node, t_token **token_ptr,
		t_context *ctx)
{
	t_redir_type	redir_type;

	if (!token_ptr || !*token_ptr)
		return (-1);
	redir_type = get_redir_type((*token_ptr)->type);
	if (redir_type == (t_redir_type)-1)
	{
		set_exit_code(ctx, ERR_SYNTAX, "minishell: invalid redirection\n");
		*token_ptr = (*token_ptr)->next;
		return (-1);
	}
	if (process_redir_token(node, token_ptr, ctx) != 0)
		return (-1);
	return (0);
}

static int	process_token(t_exec *node, t_token **token_ptr, t_context *ctx)
{
	if ((*token_ptr)->type == TOKEN_WORD)
	{
		if (process_word_token(node, token_ptr, ctx) != 0)
			return (-1);
	}
	else
	{
		if (handle_redirection(node, token_ptr, ctx) != 0)
			return (-1);
	}
	return (0);
}

static t_exec	*fill_exec_node_from_tokens(t_exec *node, t_token **token_ptr,
		t_context *ctx)
{
	if (!node || !token_ptr || !*token_ptr || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), NULL);
	while (*token_ptr && (*token_ptr)->type != TOKEN_EOF
		&& (*token_ptr)->type != TOKEN_PIPE)
	{
		if (ctx->has_syntax_error)
			return (token_failure(node, ctx), NULL);
		if (process_token(node, token_ptr, ctx) != 0)
			return (token_failure(node, ctx), NULL);
	}
	return (node);
}

static t_exec	*init_exec_node(t_context *ctx)
{
	t_exec	*new_node;

	if (!ctx)
		return (NULL);
	new_node = malloc(sizeof(t_exec));
	if (!new_node)
		return (set_exit_code(ctx, ERR_MALLOC, NULL), NULL);
	new_node->cmd = NULL;
	new_node->args = NULL;
	new_node->redirs = NULL;
	new_node->next = NULL;
	return (new_node);
}

t_exec	*create_exec_node(t_token **token_ptr, t_context *ctx)
{
	t_exec	*new_node;

	if (!token_ptr || !*token_ptr || !ctx)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), NULL);
	new_node = init_exec_node(ctx);
	if (!new_node)
		return (NULL);
	new_node = fill_exec_node_from_tokens(new_node, token_ptr, ctx);
	return (new_node);
}
