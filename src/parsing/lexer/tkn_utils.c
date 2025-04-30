/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tkn_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 20:04:31 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/26 17:03:02 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
	{
		perror("malloc failed for token");
		return (NULL);
	}
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		perror("malloc failed for token values");
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}

void	free_token_list(t_token *token_list)
{
	t_token	*current;
	t_token	*next_node;

	current = token_list;
	while (current != NULL)
	{
		next_node = current->next;
		free(current->value);
		free(current);
		current = next_node;
	}
	return ;
}

void	append_token(t_token **head, t_token **tail, t_token *token)
{
	if (*head == NULL)
	{
		*head = token;
		*tail = token;
	}
	else
	{
		(*tail)->next = token;
		*tail = token;
	}
	token->next = NULL;
	return ;
}
