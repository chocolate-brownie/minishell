/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:08:10 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/12 14:02:28 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t n)
{
	size_t	i;
	size_t	slen;

	slen = ft_strlen(src);
	i = 0;
	if (n == 0)
		return (slen);
	while (i < n - 1 && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (slen);
}
//#include <stdio.h>
//int main()
//{
//	char *s = "hello world";
//	char d[20];
//	size_t res = ft_strlcpy(d, s, 10);
//	printf("%zu\n", res);
//	printf("%s\n", d);
//	return 0;
//}
