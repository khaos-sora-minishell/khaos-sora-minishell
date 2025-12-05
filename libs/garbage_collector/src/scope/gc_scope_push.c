/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_scope_push.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:04:21 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:04:21 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/*initilaize new scope node*/

static void	gc_init_scope(t_gc_scope *scope, size_t level)
{
	scope->level = level;
	scope->first = NULL;
	scope->last = NULL;
	scope->allocation_count = 0;
	scope->prev = NULL;
}

/*
 *push new scope onto scope stack
 *creates a new scope level foe deterministic memory managment
 *returns 1 on success, 0 on failure
 */
int	gc_scope_push(t_gc_context *context)
{
	t_gc_scope	*new_scope;

	if (!context)
		return (0);
	new_scope = (t_gc_scope *)malloc(sizeof(t_gc_scope));
	if (!new_scope)
		return (0);
	gc_init_scope(new_scope, context->scope_depth);
	new_scope->prev = context->current_scope;
	context->current_scope = new_scope;
	context->scope_depth++;
	return (1);
}
