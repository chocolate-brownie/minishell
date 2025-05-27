/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:05:54 by shasinan          #+#    #+#             */
/*   Updated: 2025/05/26 16:05:50 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_id(char *args)
{
	int	i;

	if (ft_isdigit(args[0]))
		return (0);
	i = 0;
	while (args[i] && args[i] != '=')
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	handle_export_arg(t_env *env, char *arg)
{
	int		j;
	char	*id;
	char	*value;

	j = 0;
	while (arg[j] && arg[j] != '=')
		j++;
	id = ft_strndup(arg, j);
	if (!id)
		return (0);
	if (!valid_id(arg) || id[0] == '\0')
	{
		printf("export: `%s': not a valid identifier\n", arg);
		return (free(id), 0);
	}
	if (!ft_strchr(arg, '='))
		return (free(id), 0);
	value = ft_strdup(arg + j + 1);
	if (!value)
		return (free(id), 0);
	if (!update_env_var(env, id, value))
		return (free(id), free(value), 0);
	free(id);
	free(value);
	return (1);
}

int	ft_export(t_env *env, t_exec *cmd)
{
	int		i;
	char	**args;
	int		invalid_found;

	invalid_found = 0;
	args = args_to_array(cmd, 0);
	if (!args)
		return (1);
	i = 0;
	while (args[i])
	{
		if (!handle_export_arg(env, args[i]))
			invalid_found = 1;
		i++;
	}
	free_tab(args);
	if (invalid_found)
		return (1);
	else
		return (0);
}

// int	main(int ac, char **av, char **envp)
// {
// 	t_env *env;
// 	t_exec cmd;
// 	t_context ctx;

// 	(void)ac;
// 	(void)av;

// 	env = init_env(envp);
// 	cmd.cmd = "echo";
// 	cmd.next = NULL;

// 	// Création du premier nœud d'argument
// 	cmd.args = malloc(sizeof(t_args));
// 	if (!cmd.args)
// 		return (1);
// 	cmd.args->value = ft_strdup("23");
// 	cmd.args->next = malloc(sizeof(t_args));
// 	if (!cmd.args->next)
// 		return (free(cmd.args), 1);

// 	// Création du second nœud d'argument
// 	cmd.args->next->value = ft_strdup("23d");
// 	cmd.args->next->next = NULL;

// 	//ft_export(env, &cmd, &ctx);
//     //ft_unset(&env, &cmd, &ctx);
// 	// ft_exit(&cmd, &ctx, env);
// 	// printf("last exit code : %d\n", ctx.last_exit_code);
// 	//ft_env(env, &ctx);
// 	exec
// 	// Nettoyage
// 	free(cmd.args->value);
// 	free(cmd.args->next->value);
// 	free(cmd.args->next);
// 	free(cmd.args);
// 	free_env (env);
// 	return (0);
// }