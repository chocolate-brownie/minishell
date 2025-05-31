/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:35:24 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/31 18:38:58 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	redir_input(t_redirs *redir)
{
	int	fd;

	fd = open(redir->path, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->path);
		return (0);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup (input)");
		return (0);
	}
	close(fd);
	return (1);
}

int	redir_output(t_redirs *redir)
{
	int	fd;

	fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(redir->path);
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup (output)");
		return (0);
	}
	close(fd);
	return (1);
}

int	redir_append(t_redirs *redir)
{
	int	fd;

	fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->path);
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup (append)");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}
