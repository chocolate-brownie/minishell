/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:03:12 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/12 14:44:19 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dsize)
{
	size_t	i;
	size_t	dlen;
	size_t	slen;

	dlen = ft_strlen(dest);
	slen = ft_strlen(src);
	if (dsize <= dlen)
		return (dsize + slen);
	i = 0;
	while (i < (dsize - dlen - 1) && src[i])
	{
		dest[dlen + i] = src[i];
		i++;
	}
	dest[dlen + i] = '\0';
	return (dlen + slen);
}
//#include <stdio.h>
//int main()
//{
//	char s[] = "world";
//	char d[] = "hello ";
//	size_t res = ft_strlcat (d, s, 0);
//	printf("%zu\n", res);
//	printf("%s\n", d);
//}
