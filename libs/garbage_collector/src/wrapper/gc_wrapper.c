/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_wrapper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 19:54:49 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 01:45:11 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../garbage_collector.h"

void	*__wrap_malloc(size_t size)
{
	t_gc_context	*contex;

	contex = gc_get_current();
	if (!contex)
		return (__real_malloc(size));
	return (gc_malloc(contex, size));
}

void	*__wrap_calloc(size_t nmemb, size_t size)
{
	t_gc_context	*contex;

	contex = gc_get_current();
	if (!contex)
		return (__real_calloc(nmemb, size));
	return (gc_calloc(contex, nmemb, size));
}

void	*__wrap_realloc(void *ptr, size_t size)
{
	t_gc_context	*contex;

	contex = gc_get_current();
	if (!contex)
		return (__real_realloc(ptr, size));
	return (gc_realloc(contex, ptr, size));
}

void	__wrap_free(void *ptr)
{
	t_gc_context	*contex;

	contex = gc_get_current();
	if (!contex)
		__real_free(ptr);
}

/*
**	gc_get_current - placeholder function
**	Returns NULL (no global context support without global variables)
*/
t_gc_context	*gc_get_current(void)
{
	return (NULL);
}
