/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:24 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/26 18:34:58 by mgodawat         ###   ########.fr       */
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

int	main(int argc, char *argv[])
{
	char	*cmd;
	t_token	*list_head;

	if (check_state(argc, argv) == 1)
		return (1);
	while (1)
	{
		cmd = read_cmd();
		if (cmd == NULL)
			break ;
		printf("--- Input: \"%s\" ---\n", cmd);
		list_head = lexer(cmd);
		if (list_head == NULL)
		{
			write(2, "Lexer returned NULL (Error or empty input?)\n", 44);
			free(cmd);
			continue ;
		}
		print_tokens(cmd, list_head);
	}
	cmd = NULL;
	list_head = NULL;
	return (0);
}
