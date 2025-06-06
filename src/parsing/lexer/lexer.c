/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:29:42 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/06 13:07:01 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	process_token(const char *cmd, int *i, t_token **head_tail_ptrs[2],
		t_context *ctx)
{
	t_token	*new_token;

	while (cmd[*i] && ft_isspace(cmd[*i]))
		(*i)++;
	if (cmd[*i] == '\0')
		return (0);
	new_token = get_next_token(cmd, i, ctx);
	if (new_token == NULL)
	{
		if (ctx->last_exit_code != 0)
			return (-1);
		return (0);
	}
	append_token(head_tail_ptrs[0], head_tail_ptrs[1], new_token);
	return (1);
}

static int	token_creation_loop(const char *cmd, t_token **head_tail_array[2],
		t_context *ctx)
{
	int	i;
	int	result;

	i = 0;
	while (cmd[i] != '\0')
	{
		result = process_token(cmd, &i, head_tail_array, ctx);
		if (result == -1)
		{
			if (ctx->last_exit_code == ERR_UNCLOSED_QUOTE)
				set_exit_code(ctx, ERR_UNCLOSED_QUOTE, NULL);
			return (-1);
		}
		if (result == 0)
			break ;
	}
	return (0);
}

static t_token	*tokenize_input(const char *cmd, t_context *ctx)
{
	t_token	*list_head;
	t_token	*list_tail;
	t_token	**head_tail_array[2];

	init_token_list(&list_head, &list_tail);
	head_tail_array[0] = &list_head;
	head_tail_array[1] = &list_tail;
	if (token_creation_loop(cmd, head_tail_array, ctx) == -1)
	{
		free_token_list(list_head);
		return (NULL);
	}
	if (append_eof_token(&list_head, &list_tail, ctx) == -1)
		return (free_token_list(list_head), NULL);
	return (list_head);
}

/*
** The lexer function transforms the raw command string into a linked list of
** tokens.
** Each token represents a meaningful unit, such as a word, a pipe, or a
** redirection operator.
*/
t_token	*lexer(const char *cmd, t_context *ctx)
{
	t_token	*token_list;

	if (cmd == NULL || ctx == NULL)
	{
		if (ctx)
			set_exit_code(ctx, ERR_INVALID_INPUT, NULL);
		return (NULL);
	}
	token_list = tokenize_input(cmd, ctx);
	if (DEBUG && token_list)
		print_tokens((char *)cmd, token_list);
	return (token_list);
}
