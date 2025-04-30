/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 17:29:42 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/30 23:32:43 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/**
 * Takes a raw command line string and converts it into a linked list
 *        of tokens. Handles basic quoting and operator recognition.
 * @return t_token a pointer to the head of the linked list of tokens,
 *         or NULL if an error occurs (e.g., allocation failure,
		unclosed quote).
 */
t_token	*lexer(const char *cmd)
{
	t_token	*list_head;
	t_token	*list_tail;
	t_token	*new_token;
	int		i;

	new_token = NULL;
	list_head = NULL;
	list_tail = NULL;
	i = 0;
	while (cmd[i] != '\0')
	{
		while (cmd[i] && ft_isspace(cmd[i]))
			i++;
		if (cmd[i] == '\0')
			break ;
		new_token = get_next_token(cmd, &i);
		if (new_token == NULL)
		{
			free_token_list(list_head);
			return (NULL);
		}
		append_token(&list_head, &list_tail, new_token);
	}
	return (list_head);
}
