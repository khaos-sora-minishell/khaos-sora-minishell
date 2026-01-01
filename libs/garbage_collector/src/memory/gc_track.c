/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_track.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 22:02:15 by akivam            #+#    #+#             */
/*   Updated: 2025/12/08 22:30:20 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/*
 * Add metadata to global doubly-linked list
 */
static void	gc_add_to_global(t_gc_context *contex, t_gc_allocation *meta)
{
	if (!contex->all_allocations)
	{
		contex->all_allocations = meta;
		contex->all_allocations_tail = meta;
	}
	else
	{
		meta->prev = contex->all_allocations_tail;
		contex->all_allocations_tail->next = meta;
		contex->all_allocations_tail = meta;
	}
}

/*
 * Add metadata to current scope's singly-linked list
 */
static void	gc_add_to_scope(t_gc_context *contex, t_gc_allocation *meta)
{
	t_gc_scope	*scope;

	scope = contex->current_scope;
	if (!scope)
		return ;
	meta->scope_next = NULL;
	if (scope->last)
		scope->last->scope_next = meta;
	else
		scope->first = meta;
	scope->last = meta;
	scope->allocation_count++;
}

/*
 * gc_track - Register externally allocated memory with GC
 *
 * contex: garbage collector context
 * ptr: pointer from external function (getcwd, strdup, readline, etc.)
 *
 * Returns: same pointer on success, NULL on failure
 *
 * Example:
 *   char *path = getcwd(NULL, 0);
 *   gc_track(gc, path);
 */
void	*gc_track(t_gc_context *contex, void *ptr)
{
	t_gc_allocation	*meta;
	size_t			size;

	if (!contex || !ptr)
		return (NULL);
	size = gc_estimate_size(ptr);
	if (size == 0)
		size = 256;
	meta = gc_create_meta(ptr, size, contex->scope_depth);
	if (!meta)
		return (NULL);
	gc_add_to_global(contex, meta);
	if (contex->current_scope)
		gc_add_to_scope(contex, meta);
	gc_hash_add(contex, ptr, meta);
	gc_update_stats(contex, size);
	return (ptr);
}

/*
 * gc_track_sized - Track with known size
 * Use when you know exact allocation size
 *
 * Example:
 *   void *buf = malloc(1024);
 *   gc_track_sized(gc, buf, 1024);
 */
void	*gc_track_sized(t_gc_context *contex, void *ptr, size_t size)
{
	t_gc_allocation	*meta;

	if (!contex || !ptr || size == 0)
		return (NULL);
	meta = gc_create_meta(ptr, size, contex->scope_depth);
	if (!meta)
		return (NULL);
	gc_add_to_global(contex, meta);
	if (contex->current_scope)
		gc_add_to_scope(contex, meta);
	gc_hash_add(contex, ptr, meta);
	gc_update_stats(contex, size);
	return (ptr);
}

/*
 * gc_untrack - Remove pointer from GC tracking
 * Use before manual free()
 *
 * Example:
 *   char *path = getcwd(NULL, 0);
 *   gc_track(gc, path);
 *   gc_untrack(gc, path);
 *   free(path);
 */
void	gc_untrack(t_gc_context *contex, void *ptr)
{
	t_gc_allocation	*alloc;

	if (!contex || !ptr)
		return ;
	alloc = gc_find_allocation(contex, ptr);
	if (!alloc)
		return ;
	if (alloc->prev)
		alloc->prev->next = alloc->next;
	else
		contex->all_allocations = alloc->next;
	if (alloc->next)
		alloc->next->prev = alloc->prev;
	else
		contex->all_allocations_tail = alloc->prev;
	gc_hash_remove(contex, ptr);
	contex->total_freed += alloc->size;
	contex->current_usage -= alloc->size;
	contex->free_count++;
	free(alloc);
}
