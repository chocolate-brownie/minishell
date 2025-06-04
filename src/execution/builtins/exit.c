/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:13:36 by shasinan          #+#    #+#             */
/*   Updated: 2025/06/04 13:47:52 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_exit_no_args(t_context *ctx)
{
	long long	exit_status;

	exit_status = ctx->last_exit_code;
	if (isatty(STDIN_FILENO))
		printf("exit\n");
	free_all(ctx);
	exit((unsigned char)exit_status);
}

static int	handle_exit_with_args(t_exec *cmd, t_context *ctx)
{
	char		**args;
	long long	parsed_exit_code;
	int			error_flag;

	args = args_to_array(cmd, 0);
	if (!args)
		return (1);
	parsed_exit_code = ft_atol_with_error(args[0], &error_flag);
	if (!error_flag)
		return (set_error_message_and_exit_code(ctx, args, 1), 2);
	if (args[1])
	{
		if (args[1][0] != '#')
			return (set_error_message_and_exit_code(ctx, args, 2), 1);
	}
	if (isatty(STDIN_FILENO))
		printf("exit\n");
	free_tab(args);
	free_all(ctx);
	exit((unsigned char)parsed_exit_code);
	return (0);
}

int	ft_exit(t_exec *cmd, t_context *ctx)
{
	if (!cmd->args)
		return (handle_exit_no_args(ctx), 0);
	else
		return (handle_exit_with_args(cmd, ctx));
}
/*
int main(int ac, char **av, char **envp)
{
	t_exec *cmd = malloc(sizeof(t_exec));
	t_args *args = malloc(sizeof(t_args));
	t_args *args2 = malloc(sizeof(t_args));
	t_context ctx;

	(void)ac;
	(void)av;

	args2->next = NULL;
	args2->value = ft_strdup("d");
	args->next = args2;
	args->value = ft_strdup("2");
	cmd->cmd = ft_strdup("exit");
	cmd->args = args;
	cmd->next = NULL;
	cmd->redirs = NULL;
	ctx.command_list = cmd;
	ctx.envp = init_env(envp);
	ctx.last_exit_code = 0;

	ft_exit(cmd, &ctx);
	return (0);
}
 */
