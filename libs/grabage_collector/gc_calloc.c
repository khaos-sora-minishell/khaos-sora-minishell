/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:57:16 by akivam            #+#    #+#             */
/*   Updated: 2025/11/17 16:57:16 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

static void	gc_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*tmp;

	i = 0;
	tmp = s;
	while (i < n)
	{
		tmp[i] = (unsigned char)0;
		i = i + 1;
	}
}

void	*gc_calloc(size_t nmemb, size_t size)
{
	size_t	total_size;
	void	*ptr;

	if (nmemb == 0 || size == 0)
		return (NULL);
	if (size > (size_t)-1 / nmemb)
		return (NULL);
	total_size = nmemb * size;
	ptr = gc_malloc(total_size);
	if (!ptr)
		return (NULL);
	gc_bzero(ptr, total_size);
	return (ptr);
}
