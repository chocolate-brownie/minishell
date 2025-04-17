/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:12:15 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/18 10:12:17 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}
// #include <stdio.h>
// int main()
// {
//     int x = 42;
//     int x1 = 43;
//     int x2 = 44;
//     t_list *n = ft_lstnew(&x);
//     t_list *n1 = ft_lstnew(&x1);
//     t_list *n2 = ft_lstnew(&x2);

//     ft_lstadd_front(&n2, n1);
//     ft_lstadd_front(&n1, n);
//     int size = ft_lstsize(n);
//     printf ("%d\n", size);
//     return (0);
// }