/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:33:12 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/04 17:42:31 by mgodawat         ###   ########.fr       */
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

static int	is_valid_unset_identifier(const char *identifier)
{
	int	i;

	if (!identifier || identifier[0] == '\0')
		return (0);
	if (ft_isdigit(identifier[0]))
		return (0);
	i = 0;
	while (identifier[i])
	{
		if (!ft_isalnum(identifier[i]) && identifier[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	process_unset_argument(char *arg, t_env **env,
	int *return_status, int *malloc_error_flag)
{
	if (!is_valid_unset_identifier(arg))
	{
		ft_putstr_fd("minishell: unset: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		*return_status = 1;
	}
	else
	{
		if (!unset_var(env, arg))
			*malloc_error_flag = 1;
	}
}
