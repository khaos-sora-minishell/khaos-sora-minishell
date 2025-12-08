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
static void	gc_add_to_global(t_gc_context *ctx, t_gc_allocation *meta)
{
	if (!ctx->all_allocations)
	{
		ctx->all_allocations = meta;
		ctx->all_allocations_tail = meta;
	}
	else
	{
		meta->prev = ctx->all_allocations_tail;
		ctx->all_allocations_tail->next = meta;
		ctx->all_allocations_tail = meta;
	}
}

/*
 * Add metadata to current scope's singly-linked list
 */
static void	gc_add_to_scope(t_gc_context *ctx, t_gc_allocation *meta)
{
	t_gc_scope	*scope;

	scope = ctx->current_scope;
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
 * ctx: garbage collector context
 * ptr: pointer from external function (getcwd, strdup, readline, etc.)
 *
 * Returns: same pointer on success, NULL on failure
 *
 * Example:
 *   char *path = getcwd(NULL, 0);
 *   gc_track(gc, path);
 */
void	*gc_track(t_gc_context *ctx, void *ptr)
{
	t_gc_allocation	*meta;
	size_t			size;

	if (!ctx || !ptr)
		return (NULL);
	size = gc_estimate_size(ptr);
	if (size == 0)
		size = 256;
	meta = gc_create_meta(ptr, size, ctx->scope_depth);
	if (!meta)
		return (NULL);
	gc_add_to_global(ctx, meta);
	if (ctx->current_scope)
		gc_add_to_scope(ctx, meta);
	gc_update_stats(ctx, size);
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
void	*gc_track_sized(t_gc_context *ctx, void *ptr, size_t size)
{
	t_gc_allocation	*meta;

	if (!ctx || !ptr || size == 0)
		return (NULL);
	meta = gc_create_meta(ptr, size, ctx->scope_depth);
	if (!meta)
		return (NULL);
	gc_add_to_global(ctx, meta);
	if (ctx->current_scope)
		gc_add_to_scope(ctx, meta);
	gc_update_stats(ctx, size);
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
void	gc_untrack(t_gc_context *ctx, void *ptr)
{
	t_gc_allocation	*alloc;

	if (!ctx || !ptr)
		return ;
	alloc = gc_find_allocation(ctx, ptr);
	if (!alloc)
		return ;
	if (alloc->prev)
		alloc->prev->next = alloc->next;
	else
		ctx->all_allocations = alloc->next;
	if (alloc->next)
		alloc->next->prev = alloc->prev;
	else
		ctx->all_allocations_tail = alloc->prev;
	ctx->total_freed += alloc->size;
	ctx->current_usage -= alloc->size;
	ctx->free_count++;
	free(alloc);
}
