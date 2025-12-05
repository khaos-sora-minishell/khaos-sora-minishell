/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:57:17 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:25:14 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

void	*gc_realloc(void *ptr, size_t size)
{
	void		*new_ptr;
	t_collecter	*old_header;
	size_t		copy_size;

	if (!ptr)
		return (gc_malloc(size));
	if (size == 0)
		return (NULL);
	old_header = get_header_from_ptr(ptr);
	if (!old_header)
		return (NULL);
	new_ptr = gc_malloc(size);
	if (!new_ptr)
		return (NULL);
	if (old_header->size < size)
		copy_size = old_header->size;
	else
		copy_size = size;
	gc_memcpy(new_ptr, ptr, copy_size);
	return (new_ptr);
}
