/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:12:07 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/18 10:12:09 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*list;

	list = (t_list *)malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	list->content = content;
	list->next = NULL;
	return (list);
}
// #include <stdio.h>
// int main()
// {
//     int x = 42;
//     t_list *noeud;

//     noeud = ft_lstnew(&x);
//     printf("%d\n", *(int *)(noeud->content));
//     // noeud->content: caster comme pointeur vers un int
//     // dereferencer avec * pour acceder a l'int
//     free (noeud);
//     return (0);
// }