/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:46:23 by shasinan          #+#    #+#             */
/*   Updated: 2025/05/17 10:04:07 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_new_node(char *id, char *value, char *raw)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->id = id;
	new_node->value = value;
	new_node->raw = raw;
	new_node->next = NULL;
	return (new_node);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->id);
		free(env->value);
		free(env->raw);
		free(env);
		env = tmp;
	}
}

static void	free_env_all(t_env *env, char *id, char *value, char *raw)
{
	free_env(env);
	free(id);
	free(value);
	free(raw);
}

static t_env	*new_node_with_elem(char **envp, int i, int j, t_env *head)
{
	char	*id;
	char	*value;
	char	*raw;
	t_env	*node;

	id = ft_strndup(envp[i], j);
	value = ft_strdup(envp[i] + j + 1);
	raw = ft_strdup(envp[i]);
	if (!id || !value || !raw)
	{
		free_env_all(head, id, value, raw);
		return (NULL);
	}
	node = env_new_node(id, value, raw);
	if (!node)
	{
		free_env_all(head, id, value, raw);
		return (NULL);
	}
	return (node);
}

t_env	*init_env(char **envp)
{
	int		i;
	int		j;
	t_env	*head;
	t_env	*tail;
	t_env	*node;

	head = NULL;
	tail = NULL;
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		node = new_node_with_elem(envp, i, j, head);
		if (!node)
			return (NULL);
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		i++;
	}
	return (head);
}
