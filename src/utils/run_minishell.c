/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_minishell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 00:35:07 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/17 15:38:34 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Main process function that handles the lexer and the parser */
static int	process_command(char *cmd, t_token **token_list, t_context *ctx)
{
	t_exec	*exec_list;

	if (!cmd || !token_list || !ctx)
		return (-1);
	*token_list = lexer(cmd, ctx);
	if (*token_list == NULL)
		return (free(cmd), 0);
	exec_list = parser(*token_list, ctx);
	if (!exec_list)
	{
		free_token_list(*token_list);
		*token_list = NULL;
		return (free(cmd), 0);
	}
	if (DEBUG == 1)
		print_exec_list(exec_list);
	ctx->command_list = exec_list;
	execute_pipeline(ctx);
	free_exec_list(exec_list);
	ctx->command_list = NULL;
	return (1);
}

static int	manage_command_processing_outcome(int process_status,
		char *cmd_line, t_token *tokens)
{
	if (process_status == 1)
	{
		if (tokens != NULL)
			free_token_list(tokens);
		free(cmd_line);
		return (CYCLE_CONTINUE);
	}
	else if (process_status == 0)
		return (CYCLE_CONTINUE);
	else
	{
		if (cmd_line != NULL)
			free(cmd_line);
		if (tokens != NULL)
			free_token_list(tokens);
		return (CYCLE_FATAL_ERROR);
	}
}

static int	handle_command_input_cycle(t_context *ctx)
{
	char	*cmd_line;
	t_token	*tokens;
	int		process_status_code;

	tokens = NULL;
	cmd_line = read_cmd();
	if (cmd_line == NULL)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (CYCLE_BREAK_SHELL);
	}
	if ((ft_strncmp(cmd_line, "exit", 4) == 0) && (cmd_line[4] == '\0'
			|| ft_isspace(cmd_line[4])))
	{
		free(cmd_line);
		return (CYCLE_BREAK_SHELL);
	}
	process_status_code = process_command(cmd_line, &tokens, ctx);
	return (manage_command_processing_outcome(process_status_code, cmd_line,
			tokens));
}

int	run_minishell(t_context *ctx)
{
	int	cycle_status_result;
	int	final_shell_exit_code;

	if (!ctx)
		return (1);
	while (1)
	{
		cycle_status_result = handle_command_input_cycle(ctx);
		if (cycle_status_result == CYCLE_BREAK_SHELL
			|| cycle_status_result == CYCLE_FATAL_ERROR)
			break ;
	}
	final_shell_exit_code = 0;
	if (ctx)
	{
		final_shell_exit_code = ctx->last_exit_code;
		cleanup_tcontext(ctx);
	}
	else
		final_shell_exit_code = 1;
	return (final_shell_exit_code);
}
