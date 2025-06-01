/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 21:45:58 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/01 20:55:40 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define ERR_SUCCESS 0
# define ERR_MALLOC 1
# define ERR_SYNTAX 2
# define ERR_INVALID_INPUT 3
# define ERR_CMD_NOT_FOUND 127
# define ERR_PERMISSION_DENIED 126
# define ERR_PIPE 1
# define ERR_UNCLOSED_QUOTE 2
# define ERR_FAILED_REDIR 1
# define ERR_SIGINT 130
# define ERR_HD_OPEN_FAIL 1

# define QUOTE_ERROR -1
# define QUOTE_UNCLOSED -2

# define ERR_SIGACTION_FAIL 10

# define ERMSG_SIGINT "minishell: signal interruption"

# define ERMSG_SIGACTION_SIGINT "minishell: failed to set SIGINT handler"
# define ERMSG_SIGACTION_SIGQUIT "minishell: failed to set SIGQUIT handler"

# define ERMSG_INVALIDARG "minishell: invalid arguments"
# define ERMSG_UNXTKN "minishell: syntax error near unexpected token '"
# define ERMSG_DELIM_EOF "minishell: warning: hd delimited by end-of-file"

# define ERMSG_REDIR_NULLPATH "minishell: redir: NULL path"
# define ERMSG_REDIR_MALLOC "minishell: redir: malloc node"
# define ERMSG_REDIR_STRDUP "minishell: redir: ft_strdup path"
# define ERMSG_REDIR_ARGS "minishell: redir: process_redir_token args"
# define ERMSG_REDIR_UNKNOWNTOK "minishell: redir: unknown redir token"

# define ERMSG_HEREDOC_TEMP "minishell: open heredoc temp"
# define ERMSG_HEREDOC_ITOA "minishell: heredoc filename (itoa)"
# define ERMSG_HEREDOC_STRJOIN "minishell: heredoc filename (strjoin)"
# define ERMSG_HEREDOC_FEXIST "minishell: heredoc file exist"
# define ERMSG_HEREDOC_FOPEN "minishell: heredoc file open failed"

# define ERMSG_MALLOC_FAILED "minishell: memory allocation failed"

# define ERMSG_STRDUP_UQVAL "minishell: strdup in get_unquoted_val"

#endif
