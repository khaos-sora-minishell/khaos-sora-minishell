/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_sweep.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:57:00 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 19:57:00 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/*remove allocation from global list*/
static void	gc_remove_alloc(t_gc_context *contex, t_gc_allocation *alloc)
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

/*update statistics after sweeping*/
static void	gc_update_sweep_stats(t_gc_context *contex, size_t size)
{
	contex->total_freed += size;
	contex->current_usage -= size;
	contex->free_count++;
}

/*free unreachable allocation*/
static void	gc_free_unmarked(t_gc_context *contex, t_gc_allocation *alloc)
{
	gc_remove_alloc(contex, alloc);
	gc_update_sweep_stats(contex, alloc->size);
	free(alloc->ptr);
	free(alloc);
}

/*
 *sweep phase: free all unmarked(unreachable) allocations
 *removes unmarked allocations and reset marks for next collection
 */
void	gc_sweep(t_gc_context *contex)
{
	t_gc_allocation	*current;
	t_gc_allocation	*next;

	if (!contex)
		return ;
	current = contex->all_allocations;
	while (current)
	{
		next = current->next;
		if (current->marked == 0)
			gc_free_unmarked(contex, current);
		else
			current->marked = 0;
		current = next;
	}
}
