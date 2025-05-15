/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:53:08 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/15 21:46:53 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/exec.h"
#include <stdio.h>
#include <unistd.h>

static void	print_args_list(t_args *args)
{
	t_args	*curr;
	int		i;

	if (!args)
	{
		printf("\t💬 Arguments: (none)\n");
		return ;
	}
	printf("\t💬 Arguments:\n");
	curr = args;
	i = 0;
	while (curr != NULL)
	{
		printf("\t\t[%d]: \"%s\"\n", i, curr->value);
		curr = curr->next;
		i++;
	}
}

const char	*get_redirs_type_emoji(t_redir_type type)
{
	if (type == REDIR_INPUT)
		return ("< (Input)");
	else if (type == REDIR_OUTPUT)
		return ("> (Output)");
	else if (type == REDIR_APPEND)
		return (">> (Append)");
	else if (type == REDIR_HEREDOC)
		return ("<< (Heredoc)");
	else
		return ("❓ (Unknown)");
}

static void	print_redirs_list(t_redirs *redirs)
{
	t_redirs	*curr;
	int			i;
	const char	*type_str;

	if (!redirs)
	{
		printf("\t↪️ Redirections: (none)\n");
		return ;
	}
	printf("\t↪️ Redirections:\n");
	curr = redirs;
	i = 0;
	while (curr)
	{
		type_str = get_redirs_type_emoji(curr->type);
		printf("\t\t[%d]: %s Path: \"%s\"\n", i, type_str, curr->path);
		i++;
		curr = curr->next;
	}
}

void	print_exec_list(t_exec *exec_list_head)
{
	t_exec	*curr;
	int		command_num;

	command_num = 1;
	curr = exec_list_head;
	if (!curr)
		return ;
	printf("\n✨======= Execution Plan =======✨\n");
	while (curr != NULL)
	{
		printf("\n📋 Command Segment #%d:\n", command_num);
		if (curr->cmd)
		{
			printf("\t🚀 Command: \"%s\"\n", curr->cmd);
			print_args_list(curr->args);
		}
		else
			printf("\t❓ Command: (empty or invalid)\n");
		print_redirs_list(curr->redirs);
		curr = curr->next;
		command_num++;
		if (curr != NULL)
			printf("\n🔗 ----------> PIPED TO ---------->\n");
	}
	printf("\n✨========= End of Plan =========✨\n");
}
