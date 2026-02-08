/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_mark.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:57:00 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 19:57:00 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*make all allocations in a single scope*/
static void	gc_mark_scope(t_gc_scope *scope)
{
	t_gc_allocation	*current;

	if (!scope)
		return ;
	current = scope->first;
	while (current)
	{
		current->marked = 1;
		current = current->scope_next;
	}
}

/*
 *mark phase: mark all reachable allocations
 *Traverses all scopes and marks their allocations as reachable
 */
void	gc_mark(t_gc_context *contex)
{
	t_gc_scope	*scope;

	if (!contex)
		return ;
	scope = contex->current_scope;
	while (scope)
	{
		gc_mark_scope(scope);
		scope = scope->prev;
	}
}
