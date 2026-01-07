/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 19:46:29 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 19:48:10 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/*create allocation metadata (uses plain malloc - no gc tracking)*/
t_gc_allocation	*gc_meta_create(void *ptr, size_t size, size_t level,
		int from_pool)
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
	meta_data->from_pool = from_pool;
	meta_data->next = NULL;
	meta_data->prev = NULL;
	meta_data->scope_next = NULL;
	return (meta_data);
}

/* add metadata to global doubly-linked list*/
void	gc_meta_add_global(t_gc_context *contex,
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
void	gc_meta_add_scope(t_gc_context *contex, t_gc_allocation *meta_data)
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
void	gc_update_and_collecte(t_gc_context *contex, size_t size)
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
