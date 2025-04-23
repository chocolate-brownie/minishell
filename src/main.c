/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:24 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/23 21:58:27 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
*   Reading user input (using `readline`).
*   Parsing commands (you mentioned starting "lexical analysis",
	have a `src/parsing` directory).
*   Executing commands (external like `clear`, built-ins like `echo`).
*   Managing processes (`fork`, `execve`, `wait` are allowed).
*   Interacting with the terminal (`tgetent`, `tputs`,
	`ncurses`/`termcap` library).
*   Handling I/O (allowed functions include `pipe`, `dup`, `dup2`).
*/

int	main(int argc, char **argv, char **envp)
{
	(void)envp;
	if (argc > 1 || !argv[0])
		return (perr("The program accepts no arguments\n"), 1);
	if (clear_term() != 0)
		return (1);
	read_comm();
	return (0);
}
