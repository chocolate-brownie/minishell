/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 23:42:18 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/27 01:57:31 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 
struct sigaction {
    void     (*sa_handler)(int);
    void     (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t   sa_mask;     --> signals to block during your handler
    int        sa_flags;    --> e.g. SA_RESTART, SA_SIGINFO, …
    void     (*sa_restorer)(void); --> obsolete, leave NULL
};


Required Signal Behavior:

  🔸 CTRL+C (SIGINT):
  - Interactive mode: Displays a new prompt on a new line
  - During heredoc: Should behave like bash (exit heredoc, new prompt)

  🔸 CTRL+D (EOF):
  - Interactive mode: Exits the shell
  - During heredoc: Should terminate heredoc input

  🔸 CTRL+ (SIGQUIT):
  - Interactive mode: Does nothing
  - During heredoc: Should do nothing
*/

volatile sig_atomic_t	g_signal = 0;

static void	signal_handler(int sig)
{
	g_signal = sig;
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = 0;
	}
}

int	setup_signal_handlers(t_context *ctx)
{
	struct sigaction	sa;

	(void)ctx;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction(SIGINT)");
		return (-1);
	}
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction(SIGQUIT)");
		return (-1);
	}
	return (0);
}
