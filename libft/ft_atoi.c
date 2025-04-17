/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 16:14:54 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/12 16:32:04 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

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
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}
//#include <stdio.h>
//int main()
//{
//	char s[] = "    -1234dl";
//	char s1[] = "  +123";
//	char s2[] = "qgy123";
//	char s3[] = "";
//	printf("%d\n", ft_atoi(s));
//	printf("%d\n", ft_atoi(s1));
//	printf("%d\n", ft_atoi(s2));
//	printf("%d\n", ft_atoi(s3));
//	return 0;
//}
