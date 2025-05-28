/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:41:39 by shasinan          #+#    #+#             */
/*   Updated: 2025/05/15 10:37:23 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_id(char *args)
{
	int		i;
	char	*id;

	i = 0;
	while (args[i] && args[i] != '=')
		i++;
	id = ft_strndup(args, i);
	if (!id)
		return (NULL);
	return (id);
}

static void	free_node(t_env *curr)
{
	free(curr->id);
	free(curr->value);
	free(curr->raw);
	free(curr);
}

static int	unset_var(t_env **env, char *args)
{
	t_env	*curr;
	t_env	*prev;
	char	*id;

	curr = *env;
	prev = NULL;
	id = get_id(args);
	if (!id)
		return (0);
	while (curr)
	{
		if (ft_strcmp(curr->id, id) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free_node(curr);
			break ;
		}
		prev = curr;
		curr = curr->next;
	}
	free(id);
	return (1);
}

int	ft_unset(t_env **env, t_exec *cmd)
{
	char	**args;
	int		i;
	int		error;

	args = args_to_array(cmd, 0);
	if (!args)
		return (1);
	i = 0;
	error = 0;
	while (args[i])
	{
		if (!unset_var(env, args[i]))
			error = 1;
		i++;
	}
	free_tab(args);
	if (error)
		return (1);
	else
		return (0);
}
