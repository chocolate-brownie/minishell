/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:24 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/27 16:36:53 by shasinan         ###   ########.fr       */
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
your current implementation is robust in its use of termcap. */
static int	init_stds(t_context *ctx)
{
	ctx->stdin_backup = -1;
	ctx->stdout_backup = -1;
	ctx->stdin_backup = dup(STDIN_FILENO);
	ctx->stdout_backup = dup(STDOUT_FILENO);
	if (ctx->stdin_backup == -1 || ctx->stdout_backup == -1)
	{
		set_exit_code(ctx, ERR_PIPE,
			"init_context: dup failed for STDIN/STDOUT");
		if (ctx->stdin_backup != -1)
			close(ctx->stdin_backup);
		if (ctx->stdout_backup != -1)
			close(ctx->stdout_backup);
		return (-1);
	}
	return (0);
}

static t_context	*init_tcontext(char **envp)
{
	t_context	*ctx;

	ctx = malloc(sizeof(t_context));
	if (!ctx)
		return (NULL);
	ctx->last_exit_code = 0;
	ctx->should_exit = 0;
	ctx->command_list = NULL;
	ctx->pids = NULL;
	ctx->pid_count = 0;
	ctx->heredoc_counter = 0;
	ctx->active_heredocs = NULL;
	ctx->envp = NULL;
	if (init_stds(ctx) == -1)
		return (free(ctx), NULL);
	ctx->envp = populate_env_list(envp, ctx);
	if (!ctx->envp && envp && envp[0])
	{
		close(ctx->stdin_backup);
		close(ctx->stdout_backup);
		return (free(ctx), NULL);
	}
	return (ctx);
}

int	main(int argc, char **argv, char **envp)
{
	t_context	*ctx;
	int			exit_code;

	if (check_state(argc, argv) == 1)
		return (ft_putstr_fd(ERMSG_INVALIDARG, 2), ERR_INVALID_INPUT);
	ctx = init_tcontext(envp);
	if (!ctx)
		return (ft_putstr_fd("minishell: initialization failed\n", 2), 1);
	if (setup_signal_handlers(ctx) == -1)
	{
		exit_code = ctx->last_exit_code;
		cleanup_tcontext(ctx);
		free(ctx);
		return (exit_code);
	}
	exit_code = run_minishell(ctx);
	if (ctx)
		free(ctx);
	return (exit_code);
}
