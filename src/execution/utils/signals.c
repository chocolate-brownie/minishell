/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:55:37 by shasinan          #+#    #+#             */
/*   Updated: 2025/05/23 10:32:01 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
-we handle three (3) signals
-the ctrl+C : SIGINT , display a new prompt
-the ctrl+\ : SIGQUIT, do nothing
-the ctrl+D : EOF, exit*/

volatile sig_atomic_t	g_signal_received = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_received = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	setup_signal_parent(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_signal_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
