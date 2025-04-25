/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:37:56 by shasinan          #+#    #+#             */
/*   Updated: 2025/04/25 11:30:36 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd(const char *path)
{
	if (chdir(path) == -1)
	{
		printf("error changing directory");
		return ;
	}
}
/*not the last version*/