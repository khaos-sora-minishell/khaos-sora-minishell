/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:03:03 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:03:03 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <internal_collector.h>

/* check for multiplication overflow*/

static int	gc_check_overflow(size_t nmemb, size_t size, size_t *result)
{
	if (nmemb == 0 || size == 0)
		return (0);
	*result = nmemb * size;
	if (*result / nmemb != size)
		return (0);
	return (1);
}

/*
 *allocate zero-initilazed memory (calloc eqivalent)
 *nmemb: number of elements
 *size: size of each element
 *returns NULL on overflow or allocation failure
 */

void	*gc_calloc(t_gc_context *contex, size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	total_size;

	if (!contex)
		return (NULL);
	if (!gc_check_overflow(nmemb, size, &total_size))
		return (NULL);
	ptr = gc_malloc(contex, total_size);
	if (!ptr)
		return (NULL);
	gc_memset(ptr, 0, total_size);
	return (ptr);
}
