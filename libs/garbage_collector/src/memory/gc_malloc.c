/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:03:03 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:03:03 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/*create allocation metadata (uses plain malloc - no gc tracking)*/

static t_gc_allocation	*gc_meta_create(void *ptr, size_t size, size_t level)
{
	t_gc_allocation	*meta_data;

	meta_data = (t_gc_allocation *)malloc(sizeof(t_gc_allocation));
	if (!meta_data)
		return (NULL);
	gc_memset(meta_data, 0, sizeof(t_gc_allocation));
	meta_data->ptr = ptr;
	meta_data->size = size;
	meta_data->scope_level = level;
	meta_data->marked = 0;
	meta_data->next = NULL;
	meta_data->prev = NULL;
	meta_data->scope_next = NULL;
	return (meta_data);
}

/* add metadata to global doubly-linked list*/
static void	gc_meta_add_global(t_gc_context *contex,
		t_gc_allocation *meta_data)
{
	if (!contex->all_allocations)
	{
		contex->all_allocations = meta_data;
		contex->all_allocations_tail = meta_data;
	}
	else
	{
		meta_data->prev = contex->all_allocations_tail;
		contex->all_allocations_tail->next = meta_data;
		contex->all_allocations_tail = meta_data;
	}
}

/*add metadata to current scope's singly-linked list (scope_next)*/
static void	gc_meta_add_scope(t_gc_context *contex, t_gc_allocation *meta_data)
{
	t_gc_scope	*scope;

	scope = contex->current_scope;
	if (!scope)
		return ;
	meta_data->scope_next = NULL;
	if (scope->last)
		scope->last->scope_next = meta_data;
	else
		scope->first = meta_data;
	scope->last = meta_data;
	scope->allocation_count++;
}

/* update stasts and trigger auto-collection if needed*/
static void	gc_update_and_collecte(t_gc_context *contex, size_t size)
{
	contex->total_allocated += size;
	contex->current_usage += size;
	contex->allocation_count++;
	if (contex->current_usage > contex->peak_usage)
		contex->peak_usage = contex->current_usage;
	if (!contex->auto_collect)
		return ;
	if (contex->current_usage >= contex->collect_threshold)
		gc_collect(contex);
	else if (contex->allocation_count
		- contex->last_collect_count >= contex->collect_interval)
		gc_collect(contex);
}

/*
 * main gc_malloc: allocates user memory and metadata, register both
 * uses plain malloc for both allocations to void circular depency
 */

void	*gc_malloc(t_gc_context *contex, size_t size)
{
	void			*ptr;
	t_gc_allocation	*meta_data;

	if (!contex || size == 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	meta_data = gc_meta_create(ptr, size, contex->scope_depth);
	if (!meta_data)
	{
		free(ptr);
		return (NULL);
	}
	gc_meta_add_global(contex, meta_data);
	if (contex->current_scope)
		gc_meta_add_scope(contex, meta_data);
	gc_update_and_collecte(contex, size);
	return (ptr);
}
