/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:13:13 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/26 18:33:53 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

const char	*tkn_str(t_token_type type)
{
	if (type == TOKEN_WORD)
		return (GREEN "WORD" RESET);
	else if (type == TOKEN_PIPE)
		return (MAGENTA "PIPE" RESET);
	else if (type == TOKEN_REDIR_IN)
		return (BLUE "REDIR_IN" RESET);
	else if (type == TOKEN_REDIR_OUT)
		return (BLUE "REDIR_OUT" RESET);
	else if (type == TOKEN_REDIR_APPEND)
		return (YELLOW "APPEND" RESET);
	else if (type == TOKEN_REDIR_HEREDOC)
		return (WHITE "HEREDOC" RESET);
	else if (type == TOKEN_EOF)
		return (BOLDRED "EOF" RESET);
	else
		return ("UNKOWN TYPE");
}

void	print_tokens(char *cmd, t_token *list_head)
{
	t_token		*curr;
	const char	*tknstr;

	printf("----------------------------\n");
	curr = list_head;
	printf("Tokens generated:\n");
	curr = list_head;
	while (curr != NULL)
	{
		tknstr = tkn_str(curr->type);
		printf("  Token: Value=\"%s\", Type=%s\n", curr->value, tknstr);
		curr = curr->next;
	}
	free_token_list(list_head);
	printf("Token list freed.\n");
	free(cmd);
	printf("----------------------------\n");
}
