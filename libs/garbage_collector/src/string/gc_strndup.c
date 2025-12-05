/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:05:38 by akivam            #+#    #+#             */
/*   Updated: 2025/12/01 16:03:24 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*calculate string length up to n characters*/
static size_t	gc_strnlen(const char *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s[i])
		i++;
	return (i);
}

/*
 *duplicate up to n charactes of string with gc tracking
 *s: source string to duplicate
 *n: maximum number ıf characters to copy
 *return new string or NULL on faliure
 */
char	*gc_strndup(t_gc_context *contex, const char *s, size_t n)
{
	char	*duplicate;
	size_t	len;
	size_t	i;

	if (!contex || !s)
		return (NULL);
	len = gc_strnlen(s, n);
	duplicate = (char *)gc_malloc(contex, len + 1);
	if (!duplicate)
		return (NULL);
	i = 0;
	while (i < len)
	{
		duplicate[i] = s[i];
		i++;
	}
	duplicate[len] = '\0';
	return (duplicate);
}
