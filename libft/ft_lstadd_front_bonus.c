/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:11:17 by shasinan          #+#    #+#             */
/*   Updated: 2025/01/24 19:51:02 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new_node)
{
	new_node->next = *lst;
	*lst = new_node;
}
// pour pouvoir changer la valeur d'un pointeur
// on met son adresse comme argument (**list)
// void ft_lstprint(t_list *lst)
// {
//     while (lst) {
//         printf("%d ", *(int *)(lst->content));
//         lst = lst->next;
//     }
//     printf("\n");
// }
// int main()
// {
//     int x = 42;
//     int x2 = 24;
//     t_list *head = ft_lstnew(&x);
//     t_list *new = ft_lstnew(&x2);
//     ft_lstadd_front(&head, new);
//     ft_lstprint(head);
//     return (0);
// }