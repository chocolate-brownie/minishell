/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:03:01 by shasinan          #+#    #+#             */
/*   Updated: 2025/05/12 15:24:00 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_env *env)
{
	t_env	*iter;

	if (!env)
		return (1);
	iter = env;
	while (iter)
	{
		printf("%s\n", iter->raw);
		iter = iter->next;
	}
	return (0);
}
