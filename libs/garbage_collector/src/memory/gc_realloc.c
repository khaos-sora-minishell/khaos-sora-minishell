/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:03:03 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:03:03 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/* copy minimum of old and new size*/

static size_t	gc_min_size(size_t a, size_t b)
{
	if (a < b)
		return (a);
	return (b);
}

/* Handle realloc when ptr is NULL (acts like malloc) */
static void	*gc_realloc_null(t_gc_context *context, size_t size)
{
	return (gc_malloc(context, size));
}

/* Handle realloc when size is 0 (acts like free) */
static void	*gc_realloc_zero(t_gc_context *context, void *ptr)
{
	gc_free(context, ptr);
	return (NULL);
}

/*
 *reallocate memory block with new size
 *ptr: pointer to existing allocation
 *size: new size in bytes
 *returns new pointer or NULL on failure
 */

void	*gc_realloc(t_gc_context *context, void *ptr, size_t size)
{
	t_gc_allocation	*old_alloc;
	void			*new_ptr;
	size_t			copy_size;

	if (!context)
		return (NULL);
	if (!ptr)
		return (gc_realloc_null(context, size));
	if (size == 0)
		return (gc_realloc_zero(context, ptr));
	old_alloc = gc_find_allocation(context, ptr);
	if (!old_alloc)
		return (NULL);
	new_ptr = gc_malloc(context, size);
	if (!new_ptr)
		return (NULL);
	copy_size = gc_min_size(old_alloc->size, size);
	gc_memcpy(new_ptr, ptr, copy_size);
	gc_free(context, ptr);
	return (new_ptr);
}
