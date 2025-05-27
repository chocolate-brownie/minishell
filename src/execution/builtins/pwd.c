/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:05:33 by shasinan          #+#    #+#             */
/*   Updated: 2025/05/12 15:25:55 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	*path;
	char	buffer[4096];

	path = getcwd(buffer, sizeof(buffer));
	if (path)
	{
		printf("%s\n", path);
		return (0);
	}
	else
	{
		perror("cwd");
		return (1);
	}
}
