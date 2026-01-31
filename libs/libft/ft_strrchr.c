/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 09:59:45 by akivam            #+#    #+#             */
/*   Updated: 2025/06/17 18:47:27 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;
	size_t	i;

	str = (char *)s;
	i = 0;
	while (str[i] != '\0')
		i = i + 1;
	if ((char)c == '\0')
		return (str + i);
	while (i-- > 0)
	{
		if (str[i] == (char)c)
			return (str + i);
	}
	return (NULL);
}
