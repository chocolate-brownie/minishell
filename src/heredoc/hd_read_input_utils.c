/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_read_input_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:02:19 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/08 18:04:22 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "^C", 2);
}

int	handle_sigint_case_for_heredoc(char *line_read, t_context *ctx)
{
	if (line_read)
		free(line_read);
	set_exit_code(ctx, ERR_SIGINT, ERMSG_SIGINT);
	return (-1);
}

void	setup_heredoc_signals(struct sigaction *sa_heredoc_int,
							struct sigaction *sa_old_int)
{
	struct sigaction	sa_ignore;

	sigemptyset(&sa_heredoc_int->sa_mask);
	sa_heredoc_int->sa_handler = heredoc_sigint_handler;
	sa_heredoc_int->sa_flags = 0;
	sigaction(SIGINT, sa_heredoc_int, sa_old_int);
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigaction(SIGQUIT, &sa_ignore, NULL);
	g_signal = 0;
}

void	restore_old_signals(struct sigaction *sa_old_int)
{
	struct sigaction	sa_ignore;

	sigaction(SIGINT, sa_old_int, NULL);
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_handler = SIG_IGN;
	sa_ignore.sa_flags = 0;
	sigaction(SIGQUIT, &sa_ignore, NULL);
}

int	handle_heredoc_interrupt(char *line_read,
								struct sigaction *sa_old_int,
								t_context *ctx)
{
	restore_old_signals(sa_old_int);
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	return (handle_sigint_case_for_heredoc(line_read, ctx));
}
