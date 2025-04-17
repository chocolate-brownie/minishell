/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shasinan <shasinan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 09:27:31 by shasinan          #+#    #+#             */
/*   Updated: 2024/11/15 11:00:41 by shasinan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trim;
	size_t	len;
	size_t	start;
	size_t	end;
	size_t	i;

	len = ft_strlen(s1);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	if (s1[start] == '\0')
		return (ft_strdup(""));
	end = len - 1;
	while (end > start && ft_strchr(set, s1[end]))
		end--;
	trim = (char *)malloc(sizeof(char) * (end - start + 2));
	if (!trim)
		return (NULL);
	i = 0;
	while (start <= end)
	{
		trim[i++] = s1[start++];
	}
	trim[i] = '\0';
	return (trim);
}
// #include <stdio.h>
// int main()
// {
//     char *s = "-##_-hello-world-**#+--";
//     char *set = "-#+*_";
//     char *ret = ft_strtrim(s, set);
//     printf("%s\n", ret);
// }