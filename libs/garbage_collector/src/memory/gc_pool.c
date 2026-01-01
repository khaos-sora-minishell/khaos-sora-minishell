/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_pool.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 19:45:17 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 19:45:17 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/*
	create a new memory pool
	returns: new pool or NULL on failure
*/
t_gc_pool	*gc_pool_create(size_t size)
{
	t_gc_pool	*pool;

	pool = (t_gc_pool *)malloc(sizeof(t_gc_pool));
	if (!pool)
		return (NULL);
	pool->memory = malloc(size);
	if (!pool->memory)
	{
		free(pool);
		return (NULL);
	}
	pool->size = size;
	pool->used = 0;
	pool->next = NULL;
	return (pool);
}

/*
	allocate from poll (bump allocator)
	returns pointer from pool ır NULL if pool is full
	Align to 8 bytes for better performance

	*/
static void	*gc_pool_allocation_from(t_gc_pool *pool, size_t size)
{
	void	*ptr;
	size_t	aligned_size;

	if (!pool)
		return (NULL);
	aligned_size = (size + 7) & ~7;
	if (pool->used + aligned_size > pool->size)
		return (NULL);
	ptr = (char *)pool->memory + pool->used;
	pool->used += aligned_size;
	return (ptr);
}

/*
 * allocate from pool chain
 * tries existing pools, creates new if needed
 */
void	*gc_pool_alloc(t_gc_context *contex, size_t size)
{
	t_gc_pool	*pool;
	t_gc_pool	*new_pool;
	void		*ptr;

	if (!contex || size == 0 || size >= GC_SMALL_ALLOC_THRESHOLD)
		return (NULL);
	pool = contex->pool_list;
	while (pool)
	{
		ptr = gc_pool_allocation_from(pool, size);
		if (ptr)
			return (ptr);
		pool = pool->next;
	}
	new_pool = gc_pool_create(GC_POOL_SIZE);
	if (!new_pool)
		return (NULL);
	new_pool->next = contex->pool_list;
	contex->pool_list = new_pool;
	contex->pool_count++;
	return (gc_pool_allocation_from(new_pool, size));
}

/*
 * destroy all pools
 * called from gc_destroy
 */
void	gc_pool_destroy_all(t_gc_context *contex)
{
	t_gc_pool	*pool;
	t_gc_pool	*next;

	if (!contex)
		return ;
	pool = contex->pool_list;
	while (pool)
	{
		next = pool->next;
		free(pool->memory);
		free(pool);
		pool = next;
	}
	contex->pool_list = NULL;
	contex->pool_count = 0;
}
