/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_find_allocation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:06:26 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:06:26 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*find aloocation metadata by user pointer*/
t_gc_allocation	*gc_find_allocation(t_gc_context *contex, void *ptr)
{
	t_gc_allocation	*current;

	current = contex->all_allocations;
	while (current)
	{
		if (current->ptr == ptr)
			return (current);
		current = current->next;
	}
	return (NULL);
}
