/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:48:01 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/15 10:51:27 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*us;
	unsigned char		uc;
	size_t				i;

	i = 0;
	us = (unsigned char *)s;
	uc = (unsigned char)c;
	while (i < n)
	{
		if (us[i] == uc)
			return ((void *)(us + i));
		i++;
	}
	return (NULL);
}
// #include <stdio.h>
// int main()
// {
//     char s[] = "hello world";
//     char *ret = ft_memchr(s,'x', 20);
//     printf("%s\n", ret);
//     return (0);
// }