/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 09:53:59 by akivam            #+#    #+#             */
/*   Updated: 2025/06/17 18:13:00 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char	*str;
	char	*to_find;
	size_t	i;
	size_t	j;

	str = (char *)big;
	to_find = (char *)little;
	i = 0;
	if (to_find[0] == '\0')
		return (str);
	while (str[i] != '\0' && i < len)
	{
		j = 0;
		while (str[i + j] == to_find[j] && i + j < len)
		{
			if (to_find[j + 1] == '\0')
				return (&str[i]);
			j = j + 1;
		}
		i = i + 1;
	}
	return (0);
}
