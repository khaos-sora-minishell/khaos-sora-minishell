/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_collect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:57:00 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 19:57:00 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*
 *run garbage collection cycle
 *performs mark-and-sweep to free unreachable allocations,
 *updates last_collect_count for auto-collection tracking
 */

void	gc_collect(t_gc_context *contex)
{
	if (!contex)
		return ;
	gc_mark(contex);
	gc_sweep(contex);
	contex->last_collect_count = contex->allocation_count;
}
