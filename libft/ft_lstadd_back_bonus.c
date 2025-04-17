/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:11:08 by shasinan          #+#    #+#             */
/*   Updated: 2025/01/24 19:51:37 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new_node)
{
	t_list	*last;

	if (!*lst)
	{
		*lst = new_node;
	}
	else
	{
		last = ft_lstlast(*lst);
		last->next = new_node;
	}
}
// Si la liste est vide, le nouvel élément devient le premier élément
// Sinon, on ajoute le nouvel élément à la fin de la liste
// Il n'est pas nécessaire de faire new_node = NULL ici,
// car avec ft_lstnew, next est deja a NULL
// void ft_lstprint(t_list *lst)
// {
//     while (lst) {
//         printf("%d ", *(int *)(lst->content));
// Convertir content en int* et afficher la valeur
//         lst = lst->next;
//     }
//     printf("\n");
// }

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
//     int x3 = 45;
//     t_list *n3 = ft_lstnew(&x3);
//     ft_lstadd_back(&n, n3);
//     ft_lstprint(n);
//     return (0);
// }