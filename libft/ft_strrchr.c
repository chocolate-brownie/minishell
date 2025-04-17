/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:18:45 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/14 13:31:47 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;

	len = ft_strlen(s);
	if (c == '\0')
		return ((char *)(s + len));
	while (len > 0)
	{
		len--;
		if ((unsigned char)s[len] == (unsigned char)c)
			return ((char *)(s + len));
	}
	return (NULL);
}
// #include <stdio.h>
// int	main(void)
// {
// 	char *s = "hello world";
// 	printf("%s\n", ft_strrchr(s, 'l'));
// 	return (0);
// }
