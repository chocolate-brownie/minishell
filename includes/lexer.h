/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:27:13 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/25 15:46:43 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
/**
typedef enum e_token_type
{
	TOKEN_WORD, - A sequence of characters (command, argument, filename, etc.)
	TOKEN_PIPE, - |
	TOKEN_REDIR_IN, - <
	TOKEN_REDIR_OUT, - >
	TOKEN_REDIR_APPEND,	- >>
	TOKEN_REDIR_HEREDOC,- <<
	TOKEN_EOF			- End of input marker
}					t_token_type;
*/

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_EOF
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;

}					t_token;

/**
 * @brief Takes a raw command line string and converts it into a linked list
 *        of tokens. Handles basic quoting and operator recognition.
 *
 * @param input The raw command line string read from the user.
 * @return t_token* A pointer to the head of the linked list of tokens,
 *         or NULL if an error occurs (e.g., allocation failure,
		unclosed quote).
 */

t_token				*lexer(const char *input);
void				free_token_list(t_token *token_list);

#endif
