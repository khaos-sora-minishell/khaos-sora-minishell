/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:03:07 by akivam            #+#    #+#             */
/*   Updated: 2025/12/01 16:03:24 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

char	*gc_strrchr(const char *s, int c)
{
	char	*str;
	size_t	i;

	if (!s)
		return (NULL);
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
