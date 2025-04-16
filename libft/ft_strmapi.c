/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:44:47 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/15 15:00:00 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*mapi;
	int		len;

	len = ft_strlen(s);
	mapi = (char *)malloc(sizeof(char) * (len + 1));
	if (!mapi)
		return (NULL);
	i = 0;
	while (i < len)
	{
		mapi[i] = f(i, s[i]);
		i++;
	}
	mapi[i] = '\0';
	return (mapi);
}
// #include <stdio.h>
// char	map_function(unsigned int index, char c)
// {
// 	return (c + index); // Décale chaque caractère de l'indice modulo 26
// }
// int	main(void)
// {
// 	char *result = ft_strmapi("hello", map_function);
// 	if (result)
// 	{
// 		printf("Résultat : %s\n", result);
// 		free(result);
// 	}
// 	else
// 		printf("Échec de l'allocation mémoire\n");
// 	return (0);
// }
