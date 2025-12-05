/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:03:07 by akivam            #+#    #+#             */
/*   Updated: 2025/12/01 16:03:24 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

static int	gc_is_in_set(char c, const char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*gc_strtrim(t_gc_context *contex, const char *s1, const char *set)
{
	size_t	start;
	size_t	end;
	size_t	len;

	if (!contex || !s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && gc_is_in_set(s1[start], set))
		start++;
	end = gc_strlen(s1);
	while (end > start && gc_is_in_set(s1[end - 1], set))
		end--;
	len = end - start;
	return (gc_substr(contex, s1, start, len));
}
