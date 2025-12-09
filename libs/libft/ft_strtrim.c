/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 14:58:35 by akivam            #+#    #+#             */
/*   Updated: 2025/06/12 14:38:23 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	end;
	char	*trimmed;

	end = ft_strlen(s1);
	i = 0;
	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	while (s1[i] && ft_strchr(set, s1[i]))
		i = i + 1;
	while (end > i && ft_strchr(set, s1[end - 1]))
		end = end - 1;
	trimmed = (char *)malloc(end - i + 1);
	if (!trimmed)
		return (NULL);
	ft_memcpy(trimmed, s1 + i, end - i);
	trimmed[end - i] = '\0';
	return (trimmed);
}
