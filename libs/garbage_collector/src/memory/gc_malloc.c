/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 19:43:45 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 19:43:45 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/*
 * gc_allocate_raw: allocates memory from pool or malloc
 * returns the allocated pointer and sets from_pool flag
*/
static void	*gc_allocate_raw(t_gc_context *contex, size_t size, int *from_pool)
{
	void	*ptr;

	*from_pool = 0;
	if (size < GC_SMALL_ALLOC_THRESHOLD)
	{
		ptr = gc_pool_alloc(contex, size);
		if (ptr)
		{
			*from_pool = 1;
			return (ptr);
		}
	}
	return (malloc(size));
}

/*
 * main gc_malloc: allocates user memory and metadata, register both
 * uses plain malloc for both allocations to void circular depency
*/
void	*gc_malloc(t_gc_context *contex, size_t size)
{
	void			*ptr;
	t_gc_allocation	*meta_data;
	int				from_pool;

	if (!contex || size == 0)
		return (NULL);
	ptr = gc_allocate_raw(contex, size, &from_pool);
	if (!ptr)
		return (NULL);
	meta_data = gc_meta_create(ptr, size, contex->scope_depth, from_pool);
	if (!meta_data)
	{
		if (!from_pool)
			free(ptr);
		return (NULL);
	}
	gc_meta_add_global(contex, meta_data);
	if (contex->current_scope)
		gc_meta_add_scope(contex, meta_data);
	gc_hash_add(contex, ptr, meta_data);
	gc_update_and_collecte(contex, size);
	return (ptr);
}
