/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:38:45 by mgodawat          #+#    #+#             */
/*   Updated: 2025/06/04 17:38:48 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *s)
{
	int	i;

	i = 0;
	if (!s || s[0] == '\0')
		return (0);
	while (ft_isspace(s[i]))
		i++;
	if (s[i] == '+' || s[i] == '-')
		i++;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	result_loop(int sign, char *str, int i, unsigned long long *result)
{
	unsigned long long	limit;

	if (sign == 1)
		limit = LLONG_MAX;
	else
	{
		limit = (unsigned long long)LLONG_MAX + 1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		*result = (*result) * 10 + (str[i] - '0');
		if (*result > limit)
			return (0);
		i++;
	}
	return (1);
}

long long	ft_atol_with_error(char *str, int *error)
{
	int					i;
	int					sign;
	unsigned long long	result;

	*error = 1;
	if (!is_numeric(str))
		return (*error = 0, 0);
	sign = 1;
	result = 0;
	i = 0;
	while ((str[i] > 8 && str[i] < 14) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -sign;
		i++;
	}
	if (!result_loop(sign, str, i, &result))
		return (*error = 0, 0);
	return (sign * (long long)result);
}

void	set_error_message_and_exit_code(t_context *ctx, char **args, int type)
{
	if (type == 1)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_tab(args);
		free_all(ctx);
		exit(2);
	}
	if (type == 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		free_tab(args);
	}
}
