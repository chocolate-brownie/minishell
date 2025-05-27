/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 17:14:11 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/26 13:38:43 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stddef.h>
#include <string.h>

/*
typedef struct s_env
{
	char			*id;
	char			*value;
	char			*raw;
	struct s_env	*next;
}					t_env;

typedef struct s_context
{
	t_env			*envp;
	int				last_exit_code;
	int				should_exit;

	t_exec			*command_list;

	pid_t			*pids;
	int				pid_count;
	int				stdin_backup;
	int				stdout_backup;

	int				heredoc_counter;

}					t_context;
*/

/** @brief Case where there's no '=' (e.g., exported variable without 
assignment). Treat the whole string as the ID */
static int	set_id_no_equal(const char *env_str, t_env *node_ptr,
		t_context *ctx)
{
	node_ptr->id = ft_strdup(env_str);
	if (!node_ptr->id)
	{
		set_exit_code(ctx, ERR_MALLOC, "env id (no '=') strdup");
		return (-1);
	}
	node_ptr->value = NULL;
	return (0);
}

/** @brief takes the "KEY=VALUE" string seperate it and produce the id and the
value, function itself is reponsible for handling the exit_code not the caller*/
static int	set_id_val_from_equal(const char *env_str, t_env *node_ptr,
		t_context *ctx)
{
	char	*equal_sign;
	size_t	id_len;

	equal_sign = ft_strchr(env_str, '=');
	if (!equal_sign)
		return (1);
	id_len = equal_sign - env_str;
	node_ptr->id = ft_substr(env_str, 0, id_len);
	if (!node_ptr->id)
	{
		set_exit_code(ctx, ERR_MALLOC, "env id substr");
		return (-1);
	}
	node_ptr->value = ft_strdup(equal_sign + 1);
	if (!node_ptr->value)
	{
		set_exit_code(ctx, ERR_MALLOC, "env value strdup");
		free(node_ptr->id);
		node_ptr->id = NULL;
		return (-2);
	}
	return (0);
}

/** @brief creates a fresh t_env and @return a freshly created new_node. 
This function takes the 
    raw = "KEY=VALUE"
    id = "KEY"
    value = "VALUE" 
    
A) Duplicate the entire *envp string from the main and put it inside raw
B) Split into key and value
    * It looks for the = character.
    * If it finds one, everything before = becomes the id (the variable name),
    and everything after becomes the value.
    * If there is no =, it treats the entire string as the id and leaves value 
    as NULL
C) Initialize the linkage - It sets the node’s next pointer to NULL so that, 
    until you hook it into your list, it stands alone. */
static t_env	*create_env_node(const char *env_str, t_context *ctx)
{
	t_env	*new_node;
	int		result_eq;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (set_exit_code(ctx, ERR_MALLOC, "t_env malloc"), NULL);
	new_node->id = NULL;
	new_node->value = NULL;
	new_node->next = NULL;
	new_node->raw = ft_strdup(env_str);
	if (!new_node->raw)
		return (free(new_node), set_exit_code(ctx, ERR_MALLOC,
				"t_env raw strdup"), NULL);
	result_eq = set_id_val_from_equal(env_str, new_node, ctx);
	if (result_eq < 0)
		return (free(new_node->raw), free(new_node), NULL);
	if (result_eq == 1)
	{
		if (set_id_no_equal(env_str, new_node, ctx) == -1)
			return (free(new_node->raw), free(new_node), NULL);
	}
	return (new_node);
}

static void	append_node(t_env **head, t_env **tail, t_env *new_node)
{
	if (*head == NULL)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
	return ;
}

/** This function should iterate through the original envp that comes from the
main function. For each string (eg: "KEY=VALUE")
    Parse: find the '=' char to seperate the KEY from the VALUE
    Allocate a new t_env *new_node
    Duplicate and store the original envp
    Duplicate and strore id (KEY - the part before '=') 
    Duplicate and store value (VALUE)
    Handle errors
    Link the node
    Return the head */
t_env	*populate_env_list(char **original_envp, t_context *ctx)
{
	int		i;
	t_env	*new_node;
	t_env	*head;
	t_env	*current_tail;

	current_tail = NULL;
	head = NULL;
	if (!original_envp)
		return (NULL);
	i = 0;
	while (original_envp[i])
	{
		new_node = create_env_node(original_envp[i], ctx);
		if (!new_node)
			return (free_env_list(head), NULL);
		append_node(&head, &current_tail, new_node);
		i++;
	}
	return (head);
}
