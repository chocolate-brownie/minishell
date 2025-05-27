/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 00:28:03 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/22 13:51:26 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static t_redirs	*create_redir_node(t_redir_type type, char *path,
		t_context *ctx)
{
	t_redirs	*new_redir_node;

	if (!path || !ctx)
	{
		if (ctx && !path)
			set_exit_code(ctx, ERR_INVALID_INPUT, ERMSG_REDIR_NULLPATH);
		return (NULL);
	}
	new_redir_node = (t_redirs *)malloc(sizeof(t_redirs));
	if (!new_redir_node)
		return (set_exit_code(ctx, ERR_MALLOC, ERMSG_REDIR_MALLOC), NULL);
	new_redir_node->path = ft_strdup(path);
	if (!new_redir_node->path)
	{
		free(new_redir_node);
		return (set_exit_code(ctx, ERR_MALLOC, ERMSG_REDIR_STRDUP), NULL);
	}
	new_redir_node->type = type;
	new_redir_node->next = NULL;
	return (new_redir_node);
}

static t_redirs	*find_last_redir(t_redirs *head)
{
	t_redirs	*current;

	if (!head)
		return (NULL);
	current = head;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

t_redirs	*append_redir(t_redirs **redir_list_head, t_redir_type type,
		char *path, t_context *ctx)
{
	t_redirs	*new_redir_node;
	t_redirs	*last_redir;

	if (!ctx)
		return (NULL);
	if (!path)
		set_exit_code(ctx, ERR_INVALID_INPUT, ERMSG_REDIR_NULLPATH);
	new_redir_node = create_redir_node(type, path, ctx);
	if (!new_redir_node)
		return (NULL);
	if (*redir_list_head == NULL)
		*redir_list_head = new_redir_node;
	else
	{
		last_redir = find_last_redir(*redir_list_head);
		last_redir->next = new_redir_node;
	}
	return (*redir_list_head);
}
