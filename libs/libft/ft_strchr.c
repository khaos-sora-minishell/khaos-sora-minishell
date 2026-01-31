/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 09:27:47 by akivam            #+#    #+#             */
/*   Updated: 2025/06/16 18:22:39 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*str;
	size_t	i;

	str = (char *)s;
	i = 0;
	while (str[i] != '\0')
	{
		if ((unsigned char)(str[i]) == (unsigned char)c)
			return (str + i);
		i++;
	}
	if ((unsigned char)c == '\0')
		return (str + i);
	return (NULL);
}
