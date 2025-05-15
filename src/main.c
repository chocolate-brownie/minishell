/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:24 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/15 21:19:38 by mgodawat         ###   ########.fr       */
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

/* clear_term() in src/utils/clear.c: This uses termcap
(tgetent, tgetstr, tputs). While functional, termcap is an older library.
For simple screen clearing, a direct write(1, "\033[H\033[J", 7);
(ANSI escape codes for cursor home and clear screen) is often simpler and
more portable for modern terminals, avoiding termcap dependencies and potential
issues if TERM is not set or the termcap database is missing/minimal. However,
your current implementation is robust in its use of termcap.*/

static t_context	*init_tcontext(void)
{
	t_context	*ctx;

	ctx = malloc(sizeof(t_context));
	if (!ctx)
		return (set_exit_code(NULL, ERR_MALLOC, NULL), NULL);
	ctx->envp = NULL;
	ctx->last_exit_code = 0;
	ctx->should_exit = 0;
	ctx->command_list = NULL;
	ctx->pids = NULL;
	ctx->pid_count = 0;
	ctx->stdin_backup = dup(STDIN_FILENO);
	ctx->stdout_backup = dup(STDOUT_FILENO);
	if (ctx->stdin_backup == -1 || ctx->stdout_backup == -1)
		return (set_exit_code(ctx, ERR_PIPE, NULL), free(ctx), NULL);
	return (ctx);
}

/* TODO: setup_signals(); Add this function to handle signals
 * TODO: handle CTRL+D situation
    cmd = read_cmd();
    if (cmd == NULL) {
        if (errno == EINTR)
            continue ;
        break ;
    }
*/
int	main(int argc, char *argv[])
{
	t_context	*ctx;
	int			exit_code;

	ctx = NULL;
	if (check_state(argc, argv) == 1)
		return (set_exit_code(NULL, ERR_INVALID_INPUT, ERMSG_INVALIDARG), 1);
	ctx = init_tcontext();
	if (!ctx)
		return (1);
	exit_code = run_minishell(ctx);
	ctx = NULL;
	return (exit_code);
}
