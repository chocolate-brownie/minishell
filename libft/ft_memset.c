/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 09:32:44 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/15 10:51:20 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*sr;
	unsigned char	uc;

	sr = (unsigned char *)s;
	uc = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		sr[i] = uc;
		i++;
	}
	return (s);
}
//#include <stdio.h>
// int main()
//{
//	char s[] = "hello";
//	ft_memset(s, 'a', 3);
//	printf("%s\n", s);
//	return (0);
//}
