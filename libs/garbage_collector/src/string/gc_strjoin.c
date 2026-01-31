/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:05:38 by akivam            #+#    #+#             */
/*   Updated: 2025/12/01 16:03:24 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*join two strings with gc tracking*/
char	*gc_strjoin(t_gc_context *contex, const char *s1, const char *s2)
{
	char	*joined;
	size_t	len1;
	size_t	len2;
	size_t	i;

	if (!contex || !s1 || !s2)
		return (NULL);
	len1 = gc_strlen(s1);
	len2 = gc_strlen(s2);
	joined = (char *)gc_malloc(contex, len1 + len2 + 1);
	if (!joined)
		return (NULL);
	i = -1;
	while (++i < len1)
	{
		joined[i] = s1[i];
	}
	i = -1;
	while (++i < len2)
	{
		joined[len1 + i] = s2[i];
	}
	joined[len1 + len2] = '\0';
	return (joined);
}
