/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tkn_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 20:04:31 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/11 00:04:40 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new_token;

	if (!value)
		return (NULL);
	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
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

int	is_delimiter(char c)
{
	return (ft_isspace(c) || c == '|' || c == '<' || c == '>' || c == '\0');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}
