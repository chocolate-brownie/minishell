/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_code.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 23:35:59 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/11 22:04:38 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*build_formatted_error_message(const char *cmd,
											const char *details_suffix)
{
	char	*base_message;
	char	*final_message;

	base_message = ft_strjoin("minishell: ", cmd);
	if (!base_message)
		return (NULL);
	final_message = ft_strjoin(base_message, details_suffix);
	free(base_message);
	return (final_message);
}

static char	*get_cmd_specific_error(int exit_code, char *cmd)
{
	if (exit_code == ERR_CMD_NOT_FOUND)
		return (build_formatted_error_message(cmd, ": command not found\n"));
	else if (exit_code == ERR_PERMISSION_DENIED)
		return (build_formatted_error_message(cmd, ": Permission denied\n"));
	else if (exit_code == ERR_INVALID_INPUT && cmd)
		return (build_formatted_error_message(cmd, ": invalid input\n"));
	return (NULL);
}

static char	*get_syntax_error(int exit_code, char *cmd)
{
	char	*message;
	char	*temp;

	if (exit_code == ERR_SYNTAX && cmd)
	{
		message = ft_strjoin(ERMSG_UNXTKN, cmd);
		temp = message;
		message = ft_strjoin(message, "'\n");
		free(temp);
	}
	else if (exit_code == ERR_UNCLOSED_QUOTE)
		message = ft_strdup("minishell: syntax error: unclosed quote\n");
	else if (exit_code == ERR_SYNTAX)
		message = ft_strdup("minishell: syntax error\n");
	else
		message = NULL;
	return (message);
}

static char	*get_error_message(int exit_code, char *cmd)
{
	char	*message;

	message = get_cmd_specific_error(exit_code, cmd);
	if (message)
		return (message);
	message = get_syntax_error(exit_code, cmd);
	if (message)
		return (message);
	if (exit_code == ERR_MALLOC)
		message = ft_strdup("minishell: memory allocation failed\n");
	else if (exit_code == ERR_INVALID_INPUT)
		message = ft_strdup("minishell: invalid input\n");
	else if (exit_code == ERR_PIPE)
		message = ft_strdup("minishell: pipe creation failed\n");
	else
		message = ft_strdup("minishell: error\n");
	return (message);
}

void	set_exit_code(t_context *ctx, int exit_code, char *cmd)
{
	char	*error_message;

	if (!ctx)
		return ;
	ctx->last_exit_code = exit_code;
	error_message = get_error_message(exit_code, cmd);
	if (error_message)
	{
		write(2, error_message, ft_strlen(error_message));
		free(error_message);
	}
}
