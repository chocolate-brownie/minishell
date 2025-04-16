/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:36:43 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/15 14:45:25 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_nbr(int n)
{
	int	count;

	count = 0;
	if (n == 0 || n < 0)
		count++;
	while (n != 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*itoa;
	long	nb;

	nb = n;
	len = count_nbr(nb);
	itoa = (char *)malloc(sizeof(char) * (len + 1));
	if (!itoa)
		return (NULL);
	itoa[len] = '\0';
	if (nb == 0)
		itoa[0] = '0';
	if (nb < 0)
	{
		itoa[0] = '-';
		nb = -nb;
	}
	while (nb != 0)
	{
		itoa[len - 1] = (nb % 10) + '0';
		nb = nb / 10;
		len--;
	}
	return (itoa);
}
// #include <stdio.h>
// int main()
// {
//     char *ret = ft_itoa(-2147483648);

//     printf("%s\n", ret);
//     return (0);
// }
