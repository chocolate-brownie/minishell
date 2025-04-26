/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:13:13 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/26 17:31:42 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

const char	*token_type_to_string(t_token_type type)
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
