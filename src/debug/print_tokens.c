/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:13:13 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/15 21:53:11 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

const char	*tkn_str(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	else if (type == TOKEN_PIPE)
		return ("PIPE");
	else if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	else if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == TOKEN_REDIR_APPEND)
		return ("APPEND");
	else if (type == TOKEN_REDIR_HEREDOC)
		return ("HEREDOC");
	else if (type == TOKEN_EOF)
		return ("EOF");
	else
		return ("UNKNOWN TYPE");
}

void	print_tokens(char *cmd, t_token *list_head)
{
	t_token		*curr;
	const char	*token_repr;
	int			i;
	char		*value;

	if (!list_head)
	{
		printf("🤷 No tokens to display.\n");
		return ;
	}
	printf("\n🔍======= Token List =======🔍\n");
	if (cmd)
		printf("📜 Original Command: \"%s\"\n", cmd);
	curr = list_head;
	i = 0;
	while (curr != NULL)
	{
		value = curr->value;
		token_repr = tkn_str(curr->type);
		printf("\t[%d]: Value = \"%s\", Type = %s\n", i, value, token_repr);
		curr = curr->next;
		i++;
	}
	printf("🔍========= End of Tokens =========🔍\n\n");
}
