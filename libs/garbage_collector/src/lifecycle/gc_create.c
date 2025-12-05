/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_create.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:01:35 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:01:35 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

static void	gc_set_defaults(t_gc_context *ctx)
{
	ctx->mode = GC_MODE_HYBRID;
	ctx->debug_mode = GC_FALSE;
	ctx->auto_collect = GC_FALSE;
	ctx->collect_threshold = 10485760;
	ctx->collect_interval = 1000;
	ctx->last_collect_count = 0;
}

/*
**	create and initialize garbage collector context
**	automatically creates root scope for non-scoped allocations
**	Return: new context or NULL on failure
*/
t_gc_context	*gc_create(void)
{
	t_gc_context	*ctx;

	ctx = (t_gc_context *)malloc(sizeof(t_gc_context));
	if (!ctx)
		return (NULL);
	gc_memset(ctx, 0, sizeof(t_gc_context));
	gc_set_defaults(ctx);
	if (!gc_scope_push(ctx))
	{
		free(ctx);
		return (NULL);
	}
	return (ctx);
}
