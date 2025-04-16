/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:55:29 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/15 10:03:12 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;
	int	slen;

	slen = ft_strlen(s);
	i = 0;
	if (c == '\0')
		return ((char *)(s + slen));
	while (s[i])
	{
		if ((unsigned char)s[i] == (unsigned char)c)
			return ((char *)(s + i));
		i++;
	}
	return (NULL);
}
//#include <stdio.h>
//int main()
//{
//	char *s = "hello world";
//	char *ret = ft_strchr(s, 'w');
//	printf("%s\n", ret);
//	return 0;
//}
