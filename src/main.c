/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:24 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/09 03:14:53 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
*   Reading user input (using `readline`).
*   Parsing commands ("lexical analysis", have a `src/parsing` directory).
*   Executing commands (external like `clear`, built-ins like `echo`).
*   Managing processes (`fork`, `execve`, `wait` are allowed).
*   Interacting with the terminal (`tgetent`, `tputs`,
	`ncurses`/`termcap` library).
*   Handling I/O (allowed functions include `pipe`, `dup`, `dup2`).
*/

/* TODO: clear_term() in src/utils/clear.c: This uses termcap
(tgetent, tgetstr, tputs). While functional, termcap is an older library.
For simple screen clearing, a direct write(1, "\033[H\033[J", 7);
(ANSI escape codes for cursor home and clear screen) is often simpler and
more portable for modern terminals, avoiding termcap dependencies and potential
issues if TERM is not set or the termcap database is missing/minimal. However,
your current implementation is robust in its use of termcap.*/

int	main(int argc, char *argv[])
{
	char	*cmd;
	t_token	*token_list;

	if (check_state(argc, argv) == 1)
		return (1);
	while (1)
	{
		cmd = read_cmd();
		if (cmd == NULL)
			break ;
		printf("\n--- Input: \"%s\" ---\n\n", cmd);
		token_list = lexer(cmd);
		if (token_list == NULL)
		{
			write(2, "Lexer returned NULL (Error or empty input?)\n", 44);
			free(cmd);
			continue ;
		}
		parser(token_list);
		print_tokens(cmd, token_list);
	}
	cmd = NULL;
	token_list = NULL;
	return (0);
}
