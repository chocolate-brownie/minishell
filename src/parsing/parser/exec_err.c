/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:07:06 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/09 03:12:23 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	token_failure(t_exec *new_node)
{
	free_single_exec_node_content(new_node);
	free(new_node);
}

void	unexpected_token(t_exec *new_node, t_token *curr_tok)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (curr_tok && curr_tok->value)
		write(2, curr_tok->value, ft_strlen(curr_tok->value));
	else if (curr_tok)
		ft_putstr_fd("(null token value)", 2);
	else
		ft_putstr_fd("(unknown)", 2);
	ft_putstr_fd("\n", 2);
	token_failure(new_node);
}

t_redir_type	get_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_INPUT);
	if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUTPUT);
	if (type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	if (type == TOKEN_REDIR_HEREDOC)
		return (REDIR_HEREDOC);
	return (-1);
}
