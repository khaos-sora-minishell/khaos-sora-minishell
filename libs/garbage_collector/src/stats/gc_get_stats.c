/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_get_stats.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:04:56 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:04:56 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*copy statistics to output structure*/
static void	gc_copy_stats(t_gc_stats *stats, t_gc_context *contex)
{
	stats->total_allocated = contex->total_allocated;
	stats->total_freed = contex->total_freed;
	stats->current_usage = contex->current_usage;
	stats->peak_usage = contex->peak_usage;
	stats->allocation_count = contex->allocation_count;
	stats->free_count = contex->free_count;
	stats->scope_depth = contex->scope_depth;
}

/*
 *get current gc statistics
 *stats: pointer to stats structure to fill
 */

void	gc_get_stats(t_gc_context *contex, t_gc_stats *stats)
{
	if (!contex || !stats)
		return ;
	gc_copy_stats(stats, contex);
}
