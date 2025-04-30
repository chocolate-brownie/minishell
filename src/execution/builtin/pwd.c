/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:05:33 by shasinan          #+#    #+#             */
/*   Updated: 2025/04/30 09:47:18 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(void)
{
	char	*path;
	char	buffer[4096];

	path = getcwd(buffer, sizeof(buffer));
	if (path)
		printf("%s\n", path);
	else
		perror("cwd");
}
