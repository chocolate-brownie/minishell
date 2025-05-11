/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:13:13 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/10 23:50:46 by mgodawat         ###   ########.fr       */
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

void	print_tokens(char *cmd, t_token *list_head, t_context *ctx)
{
	t_token		*curr;
	const char	*tknstr;

	if (!cmd || !list_head || !ctx)
		return ;
	write(1, "----------------------------\n", 29);
	curr = list_head;
	write(1, "Tokens generated:\n", 18);
	while (curr != NULL)
	{
		tknstr = tkn_str(curr->type);
		write(1, "  Token: Value=\"", 16);
		write(1, curr->value, ft_strlen(curr->value));
		write(1, "\", Type=", 8);
		write(1, tknstr, ft_strlen(tknstr));
		write(1, "\n", 1);
		curr = curr->next;
	}
	write(1, "----------------------------\n", 29);
}
