/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:57:16 by akivam            #+#    #+#             */
/*   Updated: 2025/11/17 20:34:26 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

void	*gc_malloc(size_t size)
{
	t_collecter	**head_ptr;
	t_collecter	*new_header;

	if (size == 0)
		return (NULL);
	new_header = malloc(sizeof(t_collecter) + size);
	if (!new_header)
		return (NULL);
	head_ptr = get_gc_head();
	new_header->is_marked = 0;
	new_header->size = size;
	new_header->next = *head_ptr;
	*head_ptr = new_header;
	return ((void *)(new_header + 1));
}
