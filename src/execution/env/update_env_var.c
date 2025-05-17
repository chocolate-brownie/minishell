/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 16:52:42 by shasinan          #+#    #+#             */
/*   Updated: 2025/05/15 12:31:27 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env, char *id)
{
	char	*value;

	while (env)
	{
		if (ft_strcmp(env->id, id) == 0)
		{
			value = ft_strdup(env->value);
			if (!value)
				return (NULL);
			return (value);
		}
		env = env->next;
	}
	return (NULL);
}

static char	*dup_raw(char *id, char *new_value_dup)
{
	char	*new_raw;
	char	*id_with_equal;

	id_with_equal = ft_strjoin(id, "=");
	if (!id_with_equal)
		return (NULL);
	new_raw = ft_strjoin(id_with_equal, new_value_dup);
	free(id_with_equal);
	if (!new_raw)
		return (NULL);
	return (new_raw);
}

static int	add_if_not_there(t_env *env, char *id, char *new_value)
{
	char	*new_id;
	char	*new_value_dup;
	char	*new_raw;
	t_env	*new_node_var;
	t_env	*iter;

	new_id = ft_strdup(id);
	if (!new_id)
		return (0);
	new_value_dup = ft_strdup(new_value);
	if (!new_value_dup)
		return (free(new_id), 0);
	new_raw = dup_raw(new_id, new_value_dup);
	if (!new_raw)
	{
		return (free(new_value_dup), free(new_id), 0);
	}
	new_node_var = env_new_node(new_id, new_value_dup, new_raw);
	if (!new_node_var)
		return (free(new_id), free(new_value_dup), free(new_raw), 0);
	iter = env;
	while (iter->next)
		iter = iter->next;
	iter->next = new_node_var;
	return (1);
}

int	update_env_var(t_env *env, char *id, char *new_value)
{
	char	*new_value_dup;
	char	*new_raw;
	t_env	*iter;

	iter = env;
	while (iter)
	{
		if (ft_strcmp(iter->id, id) == 0)
		{
			new_value_dup = ft_strdup(new_value);
			if (!new_value_dup)
				return (0);
			new_raw = dup_raw(id, new_value_dup);
			if (!new_raw)
				return (free(new_value_dup), 0);
			free(iter->value);
			free(iter->raw);
			iter->value = new_value_dup;
			iter->raw = new_raw;
			return (1);
		}
		iter = iter->next;
	}
	return (add_if_not_there(env, id, new_value));
}
