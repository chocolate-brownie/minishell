/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_active_list_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:19:36 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/09 18:48:32 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
typedef struct s_hd_temp_file
{
	char					*path;
	struct s_hd_temp_file	*next;
}							t_hd_temp_file;

typedef struct s_heredoc_data
{
	char					*delimiter_str;
	char					*temp_filepath;
	int						fd;
}			t_heredoc_data;
*/
void	add_active_heredoc(t_context *ctx, const char *filepath)
{
	t_hd_temp_file	*new_node;
	t_hd_temp_file	*current;

	if (!ctx || !filepath)
		return ;
	new_node = (t_hd_temp_file *)malloc(sizeof(t_hd_temp_file));
	if (!new_node)
		return ;
	new_node->path = ft_strdup(filepath);
	if (!new_node->path)
	{
		free(new_node);
		return ;
	}
	new_node->next = NULL;
	if (ctx->active_heredocs == NULL)
		ctx->active_heredocs = new_node;
	else
	{
		current = ctx->active_heredocs;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

void	remove_and_unlink_active_heredoc(t_context *ctx, const char *filepath)
{
	t_hd_temp_file	*current;
	t_hd_temp_file	*prev;
	size_t			n;

	if (!ctx || !filepath || !ctx->active_heredocs)
		return ;
	current = ctx->active_heredocs;
	prev = NULL;
	while (current != NULL)
	{
		n = ft_strlen(filepath) + 1;
		if (ft_strncmp(current->path, filepath, n) == 0)
		{
			if (prev == NULL)
				ctx->active_heredocs = current->next;
			else
				prev->next = current->next;
			unlink(current->path);
			free(current->path);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	cleanup_all_active_heredocs(t_context *ctx)
{
	t_hd_temp_file	*current;
	t_hd_temp_file	*next_node;

	if (!ctx || !ctx->active_heredocs)
		return ;
	current = ctx->active_heredocs;
	while (current != NULL)
	{
		next_node = current->next;
		unlink(current->path);
		free(current->path);
		free(current);
		current = next_node;
	}
	ctx->active_heredocs = NULL;
}
