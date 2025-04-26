/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:24 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/26 17:19:17 by mgodawat         ###   ########.fr       */
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
	t_token	*current;

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
			printf(RED "Lexer returned NULL (Error or empty input?)\n" RESET);
			free(cmd);
			continue ;
		}
		printf("Tokens generated:\n");
		current = list_head;
		while (current != NULL)
		{
			printf("  Token: Value=\"%s\", Type=%s\n", current->value,
					token_type_to_string(current->type));
			current = current->next;
		}
		free_token_list(list_head);
		printf("Token list freed.\n");
		free(cmd);
		printf("----------------------------\n");
	}
	cmd = NULL;
	list_head = NULL;
	return (0);
}
