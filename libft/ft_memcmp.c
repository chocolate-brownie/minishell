/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:19:08 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/15 10:49:55 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*u1;
	const unsigned char	*u2;

	if (n == 0)
		return (0);
	u1 = (const unsigned char *)s1;
	u2 = (const unsigned char *)s2;
	i = 0;
	while (i < n && u1[i] == u2[i])
	{
		i++;
	}
	if (i == n)
		return (0);
	return (u1[i] - u2[i]);
}
//#include <stdio.h>
// int main()
//{
//	char *s1 = "hello";
//	char *s2 = "hellp";
//	int ret = ft_memcmp(s1, s2, 0);
//	printf("%d\n", ret);
//	return (0);
//}
