/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_read_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 00:39:07 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/31 15:44:50 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	perform_read_loop(char *buffer, int *read_len)
{
	char	c;
	int		ret;
	int		current_len;

	current_len = 0;
	while (current_len < 1023)
	{
		ret = read(STDIN_FILENO, &c, 1);
		if (g_signal == SIGINT)
			return (1);
		if (ret == 0)
		{
			*read_len = current_len;
			return (2);
		}
		if (c == '\n')
		{
			*read_len = current_len;
			return (0);
		}
		buffer[current_len++] = c;
	}
	*read_len = current_len;
	return (0);
}

static char	*read_line_manual(t_context *ctx)
{
	char	*buffer;
	int		len;
	int		loop_status;

	write(STDOUT_FILENO, "heredoc> ", 9);
	buffer = malloc(1024);
	if (!buffer)
	{
		set_exit_code(ctx, ERR_MALLOC, "malloc at read_line_manual");
		return (NULL);
	}
	len = 0;
	loop_status = perform_read_loop(buffer, &len);
	if (loop_status == 1)
	{
		free(buffer);
		return (NULL);
	}
	if (loop_status == 2 && len == 0)
	{
		free(buffer);
		return (NULL);
	}
	buffer[len] = '\0';
	return (buffer);
}

static int	process_single_heredoc_line(char *line_read,
										t_heredoc_data *hdata,
										t_context *ctx)
{
	size_t	len_line;
	size_t	len_delim;

	if (!line_read)
		return (0);
	len_line = ft_strlen(line_read);
	len_delim = ft_strlen(hdata->delimiter_str);
	if (len_line == len_delim
		&& ft_strncmp(line_read, hdata->delimiter_str, len_delim) == 0)
		return (free(line_read), 0);
	if (write(hdata->fd, line_read, len_line) == -1)
	{
		set_exit_code(ctx, ERR_PIPE, "heredoc write failed");
		return (free(line_read), -1);
	}
	if (write(hdata->fd, "\n", 1) == -1)
	{
		set_exit_code(ctx, ERR_PIPE, "heredoc write failed");
		return (free(line_read), -1);
	}
	return (free(line_read), 1);
}

// NEW minimal signal handler for use only during heredoc's manual read
static void heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT; // ONLY set the flag
}

// Modified version of handle_sigint_case for the new heredoc-specific handler
static int	handle_sigint_case_for_heredoc(char *line_read, t_context *ctx)
{
	if (line_read) // If a line was partially read before signal
		free(line_read);
	set_exit_code(ctx, ERR_SIGINT, ERMSG_SIGINT); // Set appropriate exit code
	return (-1); // Indicate error/interruption
}

int	read_heredoc_input(t_heredoc_data *hdata, t_context *ctx)
{
	char				*line_read;
	int					process_rc;
	struct sigaction	sa_heredoc_int;
	struct sigaction	sa_old_int;
	// struct sigaction	sa_ignore_quit; // Optional: if you want to explicitly ignore SIGQUIT
	// struct sigaction	sa_old_quit;    // Optional: and restore it

	// 1. Setup minimal SIGINT handler for heredoc duration
	sigemptyset(&sa_heredoc_int.sa_mask);
	sa_heredoc_int.sa_handler = heredoc_sigint_handler;
	sa_heredoc_int.sa_flags = 0; 
	sigaction(SIGINT, &sa_heredoc_int, &sa_old_int);

	// Optional: Ignore SIGQUIT during heredoc input
	// sigemptyset(&sa_ignore_quit.sa_mask);
	// sa_ignore_quit.sa_handler = SIG_IGN;
	// sa_ignore_quit.sa_flags = 0;
	// sigaction(SIGQUIT, &sa_ignore_quit, &sa_old_quit);

	g_signal = 0; // Reset our flag specifically for this heredoc reading session
	while (1)
	{
		line_read = read_line_manual(ctx); // This prints "heredoc> "

		if (g_signal == SIGINT) // Check flag set by heredoc_sigint_handler
		{
			sigaction(SIGINT, &sa_old_int, NULL); // Restore original SIGINT handler
			// if (sa_old_quit.sa_handler != NULL) // Restore SIGQUIT if changed
			// 	sigaction(SIGQUIT, &sa_old_quit, NULL);

			// Manually perform actions to prepare readline state, but don't redisplay here.
			write(STDOUT_FILENO, "\n", 1);
			rl_replace_line("", 0); 
			rl_on_new_line(); 
			
			return (handle_sigint_case_for_heredoc(line_read, ctx));
		}
		if (!line_read) // EOF case (Ctrl+D on empty line)
		{
			sigaction(SIGINT, &sa_old_int, NULL);
			// if (sa_old_quit.sa_handler != NULL)
			// 	sigaction(SIGQUIT, &sa_old_quit, NULL);
			return (handle_eof_case(hdata)); // Make sure handle_eof_case is defined
		}
		process_rc = process_single_heredoc_line(line_read, hdata, ctx);
		if (process_rc <= 0) // Delimiter found or write error
		{
			sigaction(SIGINT, &sa_old_int, NULL);
			// if (sa_old_quit.sa_handler != NULL)
			// 	sigaction(SIGQUIT, &sa_old_quit, NULL);
			return (process_rc);
		}
	}
	// Safeguard, though loop should always exit via return
	sigaction(SIGINT, &sa_old_int, NULL);
	// if (sa_old_quit.sa_handler != NULL)
	// 	sigaction(SIGQUIT, &sa_old_quit, NULL);
	return (0); 
}
