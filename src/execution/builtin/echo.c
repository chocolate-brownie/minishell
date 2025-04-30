/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 19:10:03 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/30 09:44:17 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tab(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

int	valid_option(char **args, int i)
{
	int	j;

	if (args[i][0] != '-')
		return (0);
	j = 1;
	while (args[i][j])
	{
		if (args[i][j] == 'n')
			j++;
		else
			break ;
	}
	if (args[i][j] == '\0')
		return (1);
	else
		return (0);
}

void	ft_echo(char **args)
{
	int	i;
	int	option;

	i = 1;
	option = 0;
	while (args[i] && valid_option(args, i) == 1)
	{
		option = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!option)
		write(1, "\n", 1);
}

// int	main(int ac, char **av, char **envp)
// {
// 	char	*line;
// 	char	**args;
// 	int pid;
// 	t_env *env;

// 	(void)ac;
// 	(void)av;
// 	while (1)
// 	{
// 		line = readline("minishell>");
// 		if (!line)
// 		{
// 			printf("exit\n");
// 			break ;
// 		}
// 		if (*line)
// 			add_history(line);
// 		args = ft_split(line, ' ');
// 		if (args && args[0])
// 		{
// 			if (args && args[0] && (strcmp(args[0], "echo") == 0))
// 				ft_echo(args);
// 			else if ((strcmp(args[0], "cd") == 0))
// 				ft_cd(args[1]);
// 			else if ((strcmp(args[0], "pwd") == 0))
// 				ft_pwd();
// 			else if ((strcmp(args[0], "env") == 0))
// 			{
// 				env = init_env(envp);
// 				ft_env(env);
// 			}
// 			else if (strcmp(args[0], "clear") == 0)
// 				clear_term();
// 			else if (strcmp(line, "ls -l") == 0)
// 			{
// 				pid = fork();
// 				if (pid == 0)
// 				{
// 					execve("/usr/bin/ls", args, envp);
// 				}
// 				else
// 					wait(NULL);
// 			}
// 		}
// 		free(line);
// 		free_tab(args);
// 	}
// 	return (0);
// }
