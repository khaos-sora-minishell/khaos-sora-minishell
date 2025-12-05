/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_scope_pop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:04:21 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:04:21 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/*free single allocation and its metadata*/
static void	gc_free_allocation(t_gc_allocation *alloc)
{
	if (!alloc)
		return ;
	free(alloc->ptr);
	free(alloc);
}

/*remove allocation from global doublt-linked list*/
static void	gc_remove_global(t_gc_context *contex, t_gc_allocation *alloc)
{
	if (alloc->prev)
		alloc->prev->next = alloc->next;
	else
		contex->all_allocations = alloc->next;
	if (alloc->next)
		alloc->next->prev = alloc->prev;
	else
		contex->all_allocations_tail = alloc->prev;
}

/*update statistics when freeing allocation*/
static void	gc_update_stats(t_gc_context *contex, size_t size)
{
	contex->total_freed += size;
	contex->current_usage -= size;
	contex->free_count++;
}

/*free all allocations in current scope*/
static void	gc_free_scope_allocs(t_gc_context *contex, t_gc_scope *scope)
{
	t_gc_allocation	*current;
	t_gc_allocation	*next;

	current = scope->first;
	while (current)
	{
		next = current->scope_next;
		gc_remove_global(contex, current);
		gc_update_stats(contex, current->size);
		gc_free_allocation(current);
		current = next;
	}
}

/*
 *pop current scope from stack and free all its allocations
 *deterministic memıry cleanup for scope-based managment
 */
void	gc_scope_pop(t_gc_context *contex)
{
	t_gc_scope	*scope_to_free;

	if (!contex || !contex->current_scope)
		return ;
	scope_to_free = contex->current_scope;
	gc_free_scope_allocs(contex, scope_to_free);
	contex->current_scope = scope_to_free->prev;
	contex->scope_depth--;
	free(scope_to_free);
}
