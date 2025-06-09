/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:50:18 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/09 22:44:04 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/* check whether the current token is EOF or if its NULL if it is return NULL
 * check whether the beginning of the command is  PIPE if it is free the
 * token_list and throw NULL
 * go through each token != NULL or EOF and process create the exec struct
 * reagarding to the corresponding token value after the process it should
 * return the pointer to the current token so I can check whether its pointing
 * to a PIPE at this point I have created a full t_execv */
static int	process_segment(t_token **curr_tok_ptr, t_exec **head_ptr,
		t_exec **tail_ptr, t_context *ctx)
{
	t_exec	*new_exec;
	t_token	*start_segment_token;

	if (g_signal == SIGINT)
		return (SEGMENT_ERROR_CLEANUP_MAIN);
	start_segment_token = *curr_tok_ptr;
	new_exec = create_exec_node(curr_tok_ptr, ctx);
	if (g_signal == SIGINT)
	{
		if (new_exec)
			free_single_exec_node_content(new_exec);
		return (SEGMENT_ERROR_CLEANUP_MAIN);
	}
	if (new_exec == NULL)
		return (SEGMENT_ERROR_CLEANUP_MAIN);
	if (*head_ptr != NULL && new_exec->cmd == NULL && new_exec->redirs == NULL)
		return (invalseg_after_pipe(head_ptr, &new_exec, &start_segment_token,
				ctx), SEGMENT_ERROR_CLEANUP_DONE);
	link_nodes(head_ptr, tail_ptr, new_exec);
	if (check_next_tok(curr_tok_ptr, ctx) != 0)
		return (SEGMENT_ERROR_CLEANUP_MAIN);
	return (SEGMENT_PROCESSED_CONTINUE);
}

static t_exec	*handle_segment_status(int status, t_exec *head_node)
{
	if (status == SEGMENT_ERROR_CLEANUP_MAIN)
		return (free_exec_list(head_node), NULL);
	if (status == SEGMENT_ERROR_CLEANUP_DONE)
		return (NULL);
	if (status != SEGMENT_PROCESSED_CONTINUE)
		return (free_exec_list(head_node), NULL);
	return (head_node);
}

static t_exec	*parse_token_loop(t_token *current_token, t_context *ctx)
{
	t_exec	*head_node;
	t_exec	*tail_node;
	int		status;
	t_exec	*result;

	head_node = NULL;
	tail_node = NULL;
	while (current_token && current_token->type != TOKEN_EOF)
	{
		if (g_signal == SIGINT || ctx->has_syntax_error)
			return (free_exec_list(head_node), NULL);
		status = process_segment(&current_token, &head_node, &tail_node, ctx);
		if (g_signal == SIGINT)
			return (free_exec_list(head_node), NULL);
		result = handle_segment_status(status, head_node);
		if (result != head_node)
			return (result);
	}
	return (head_node);
}

t_exec	*parser(t_token *token_list, t_context *ctx)
{
	t_exec	*result_exec_list;

	if (g_signal == SIGINT)
		return (NULL);
	if (ctx == NULL || validate_init_tokens(token_list, ctx) != 0)
	{
		return (NULL);
	}
	result_exec_list = parse_token_loop(token_list, ctx);
	return (result_exec_list);
}
