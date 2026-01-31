/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_find_allocation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 19:53:04 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 19:53:04 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*
	find aloocation metadata by user pointer
	now uses hash table for O(1) lookup instead of O(n) linear search
*/
t_gc_allocation	*gc_find_allocation(t_gc_context *contex, void *ptr)
{
	if (!contex || !ptr)
		return (NULL);
	return (gc_hash_find(contex, ptr));
}
