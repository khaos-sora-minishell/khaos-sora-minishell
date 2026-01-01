/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:03:03 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:03:03 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/* remove allocation from global doubly-linked list*/
static void	gc_remove_from_list(t_gc_context *context, t_gc_allocation *alloc)
{
	if (alloc->prev)
		alloc->prev->next = alloc->next;
	else
		context->all_allocations = alloc->next;
	if (alloc->next)
		alloc->next->prev = alloc->prev;
	else
		context->all_allocations_tail = alloc->prev;
}

/* update statistics after freeing*/
static void	gc_update_free_stats(t_gc_context *context, size_t size)
{
	context->total_freed += size;
	context->current_usage -= size;
	context->free_count++;
}

/*
 *main gc_free: manualyy free allocated memory
 *removes metadata and updates statistics
 */

void	gc_free(t_gc_context *context, void *ptr)
{
	t_gc_allocation	*alloc;

	if (!context || !ptr)
		return ;
	alloc = gc_find_allocation(context, ptr);
	if (!alloc)
		return ;
	gc_remove_from_list(context, alloc);
	gc_hash_remove(context, ptr);
	gc_update_free_stats(context, alloc->size);
	if (alloc->from_pool == 0)
		free(alloc->ptr);
	free(alloc);
}
