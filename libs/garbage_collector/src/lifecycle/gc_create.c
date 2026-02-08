/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_create.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/11/28 20:01:35 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:01:35 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

static void	gc_set_defaults(t_gc_context *contex)
{
	contex->mode = GC_MODE_HYBRID;
	contex->debug_mode = GC_FALSE;
	contex->auto_collect = GC_FALSE;
	contex->collect_threshold = 10485760;
	contex->collect_interval = 1000;
	contex->last_collect_count = 0;
}

static void	gc_init_hash_table(t_gc_context *contex)
{
	size_t	i;

	i = -1;
	while (++i < GC_HASH_SIZE)
		contex->hash_table[i] = NULL;
}

/*
**	create and initialize garbage collector context
**	automatically creates root scope for non-scoped allocations
**	Return: new context or NULL on failure
*/
t_gc_context	*gc_create(void)
{
	t_gc_context	*contex;

	contex = (t_gc_context *)malloc(sizeof(t_gc_context));
	if (!contex)
		return (NULL);
	gc_memset(contex, 0, sizeof(t_gc_context));
	gc_init_hash_table(contex);
	gc_set_defaults(contex);
	contex->pool_list = gc_pool_create(GC_POOL_SIZE);
	if (!gc_scope_push(contex))
	{
		gc_pool_destroy_all(contex);
		free(contex);
		return (NULL);
	}
	return (contex);
}
