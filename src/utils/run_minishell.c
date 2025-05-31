/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_minishell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 00:35:07 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/31 17:43:51 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_command(char *cmd, t_token **token_list, t_context *ctx)
{
	t_exec	*exec_list;

	if (!cmd || !token_list || !ctx)
		return (-1);
	*token_list = lexer(cmd, ctx);
	if (g_signal == SIGINT)
		return (cleanup_after_sigint(cmd, token_list));
	if (*token_list == NULL)
		return (free(cmd), 0);
	exec_list = parser(*token_list, ctx);
	if (g_signal == SIGINT)
		return (cleanup_after_sigint(cmd, token_list));
	if (!exec_list)
		return (cleanup_failed_exec(cmd, token_list));
	ctx->command_list = exec_list;
	ctx->token_list = *token_list;
	if (DEBUG && exec_list)
	{
		printf("--- AST Structure (in process_command) ---\n");
		print_exec_list(exec_list);
		printf("--- End AST Structure (in process_command) ---\n");
	}
	if (!execute_pipeline(ctx))
		return (free_exec_list(exec_list), ctx->command_list = NULL, 2);
	return (free_exec_list(exec_list), ctx->command_list = NULL, 1);
}

static int	manage_command_processing_outcome(int process_status,
												char *cmd_line,
												t_token *tokens,
												t_context *ctx)
{
	if (process_status == 1)
	{
		if (tokens != NULL)
			free_token_list(tokens);
		if (cmd_line != NULL)
			free(cmd_line);
		return (CYCLE_CONTINUE);
	}
	else if (process_status == 0)
		return (CYCLE_CONTINUE);
	else if (process_status == -2)
	{
		ctx->last_exit_code = ERR_SIGINT;
		return (CYCLE_CONTINUE);
	}
	else
	{
		if (cmd_line != NULL)
			free(cmd_line);
		if (tokens != NULL)
			free_token_list(tokens);
		return (CYCLE_FATAL_ERROR);
	}
}

static int	handle_input_cases(char *cmd_line, t_context *ctx)
{
	if (g_signal == SIGINT)
	{
		if (cmd_line)
			free(cmd_line);
		ctx->last_exit_code = ERR_SIGINT;
		return (CYCLE_CONTINUE);
	}
	if (cmd_line == NULL)
		return (CYCLE_BREAK_SHELL);
	if (cmd_line[0] == '\0')
		return (free(cmd_line), CYCLE_CONTINUE);
	return (0);
}

static int	handle_command_input_cycle(t_context *ctx)
{
	char	*cmd_line;
	t_token	*tokens;
	int		process_status_code;
	int		input_result;

	tokens = NULL;
	if (isatty(STDIN_FILENO))
		g_signal = 0;
	cmd_line = read_cmd(ctx);
	input_result = handle_input_cases(cmd_line, ctx);
	if (input_result != 0)
		return (input_result);
	process_status_code = process_command(cmd_line, &tokens, ctx);
	return (manage_command_processing_outcome(process_status_code, cmd_line,
			tokens, ctx));
}

int	run_minishell(t_context *ctx)
{
	int	cycle_status_result;
	int	final_shell_exit_code;

	if (!ctx)
		return (1);
	if (isatty(STDIN_FILENO))
		g_signal = 0;
	while (1)
	{
		cycle_status_result = handle_command_input_cycle(ctx);
		if (cycle_status_result == CYCLE_BREAK_SHELL
			|| cycle_status_result == CYCLE_FATAL_ERROR)
			break ;
	}
	final_shell_exit_code = ctx->last_exit_code;
	cleanup_tcontext(ctx);
	return (final_shell_exit_code);
}
