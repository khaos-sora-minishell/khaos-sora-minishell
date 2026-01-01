/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_track_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 22:10:00 by akivam            #+#    #+#             */
/*   Updated: 2025/12/31 20:56:44 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/*
 * Estimate size of external allocation
 * For strings: calculate length + 1
 * For other: use provided size or default
 */
size_t	gc_estimate_size(void *ptr)
{
	char	*str;
	size_t	i;

	if (!ptr)
		return (0);
	str = (char *)ptr;
	i = 0;
	while (i < 4096 && str[i])
		i++;
	if (i < 4096 && str[i] == '\0')
		return (i + 1);
	return (256);
}

/*
 * Create metadata for external allocation
 */
t_gc_allocation	*gc_create_meta(void *ptr, size_t size, size_t level)
{
	t_gc_allocation	*meta;

	meta = (t_gc_allocation *)malloc(sizeof(t_gc_allocation));
	if (!meta)
		return (NULL);
	gc_memset(meta, 0, sizeof(t_gc_allocation));
	meta->ptr = ptr;
	meta->size = size;
	meta->scope_level = level;
	meta->marked = 0;
	meta->from_pool = 0;
	meta->next = NULL;
	meta->prev = NULL;
	meta->scope_next = NULL;
	return (meta);
}

/*
 * Update context statistics
 */
void	gc_update_stats(t_gc_context *contex, size_t size)
{
	contex->total_allocated += size;
	contex->current_usage += size;
	contex->allocation_count++;
	if (contex->current_usage > contex->peak_usage)
		contex->peak_usage = contex->current_usage;
}
