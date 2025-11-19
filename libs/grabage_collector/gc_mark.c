/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_mark.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:34:08 by harici            #+#    #+#             */
/*   Updated: 2025/11/17 21:02:27 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

void	mark_pointer(void *ptr)
{
	t_collecter	**head_ptr;
	t_collecter	*node;
	void		*data_ptr;

	head_ptr = get_gc_head();
	node = *head_ptr;
	while (node)
	{
		data_ptr = (void *)(node + 1);
		if (ptr == data_ptr && !node->is_marked)
		{
			node->is_marked = 1;
			mark_memory_region(data_ptr, node->size);
			break ;
		}
		node = node->next;
	}
}

void	mark_memory_region(void *start, size_t size)
{
	size_t	i;
	void	**potential_ptr;

	i = 0;
	while (i + sizeof(void *) <= size)
	{
		potential_ptr = (void **)((char *)start + i);
		if (is_valid_pointer(*potential_ptr))
			mark_pointer(*potential_ptr);
		i += sizeof(void *);
	}
}

int	is_valid_pointer(void *ptr)
{
	t_collecter	**head_ptr;
	t_collecter	*node;
	void		*data_ptr;

	if (!ptr)
		return (0);
	head_ptr = get_gc_head();
	node = *head_ptr;
	while (node)
	{
		data_ptr = (void *)(node + 1);
		if (ptr >= data_ptr && ptr < (void *)((char *)data_ptr + node->size))
			return (1);
		node = node->next;
	}
	return (0);
}

void	gc_mark(void)
{
	mark_from_stack();
}
