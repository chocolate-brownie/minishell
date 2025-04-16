/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 15:02:11 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/15 15:49:26 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int	i;

	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
// avec l'adresse comme argument,
// la fonction peut directement changer le caractere dans la chaine
// void too_upper(unsigned int index, char *ch)
// {
//     if (*ch >= 'a' && *ch <= 'z')
//         *ch -= 32;
//     printf("Index %u: %c\n", index, *ch);
// }
// int main()
// {
//     char str[] = "hello world";
//     ft_striteri(str, too_upper);
//     printf("%s\n", str);
//     return (0);
// }