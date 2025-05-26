/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:29:42 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/24 16:02:09 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/error.h"
#include "../../../includes/minishell.h"
#include <string.h>

static int	validate_input(const char *cmd, t_context *ctx)
{
	if (!cmd)
		return (set_exit_code(ctx, ERR_INVALID_INPUT, NULL), 0);
	if (!ctx)
		return (0);
	if (cmd[0] == '\0')
		return (set_exit_code(ctx, 0, NULL), 0);
	return (1);
}

static void	init_token_list(t_token **list_head, t_token **list_tail)
{
	*list_head = NULL;
	*list_tail = NULL;
}

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
		if (ctx->last_exit_code == ERR_UNCLOSED_QUOTE)
			set_exit_code(ctx, ERR_MALLOC, "token creation in lexer");
		return (-1);
	}
	append_token(head_tail_ptrs[0], head_tail_ptrs[1], new_token);
	return (1);
}

static t_token	*tokenize_input(const char *cmd, t_context *ctx)
{
	t_token	*list_head;
	t_token	*list_tail;
	t_token	**head_tail_array[2];
	int		i;
	int		result;

	init_token_list(&list_head, &list_tail);
	head_tail_array[0] = &list_head;
	head_tail_array[1] = &list_tail;
	i = 0;
	while (cmd[i] != '\0')
	{
		result = process_token(cmd, &i, head_tail_array, ctx);
		if (result == -1)
			return (free_token_list(list_head), NULL);
		if (result == 0)
			break ;
	}
	if (append_eof_token(&list_head, &list_tail, ctx) == -1)
		return (free_token_list(list_head), NULL);
	return (list_head);
}

t_token	*lexer(const char *cmd, t_context *ctx)
{
	t_token	*lexer;

	if (!validate_input(cmd, ctx))
		return (NULL);
	lexer = tokenize_input(cmd, ctx);
	if (DEBUG == 1)
		print_tokens((char *)cmd, lexer);
	return (lexer);
}
