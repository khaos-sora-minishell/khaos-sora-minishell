/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:05:38 by akivam            #+#    #+#             */
/*   Updated: 2025/12/01 16:03:24 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*
 *duplicate string with gc tracking
 *s: source string to duplicate
 *returns new string or NULL on failure
 */
char	*gc_strdup(t_gc_context *contex, const char *s)
{
	char	*duplicate;
	size_t	len;
	size_t	i;

	if (!contex || !s)
		return (NULL);
	len = gc_strlen(s);
	duplicate = (char *)gc_malloc(contex, len + 1);
	if (!duplicate)
		return (NULL);
	i = 0;
	while (i < len)
	{
		duplicate[i] = s[i];
		i++;
	}
	duplicate[i] = '\0';
	return (duplicate);
}
