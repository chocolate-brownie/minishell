/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:11:51 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/18 10:11:53 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
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
//     t_list *ret = ft_lstlast(n);
//     printf ("%d\n", *(int *)(ret->content));
//     return (0);
// }