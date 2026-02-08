/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_set_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:00:12 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:00:12 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*
 *set garbage collection mode
 *mode: GC_MODE_MANUAL,GC_MODE_AUTO or GC_MODE_HYBRID
 */
void	gc_set_mode(t_gc_context *contex, t_gc_mode mode)
{
	if (!contex)
		return ;
	contex->mode = mode;
	if (mode == GC_MODE_AUTO || mode == GC_MODE_HYBRID)
		contex->auto_collect = 1;
	else
		contex->auto_collect = 0;
}
