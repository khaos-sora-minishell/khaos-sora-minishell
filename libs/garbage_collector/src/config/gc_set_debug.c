/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_set_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:00:12 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:00:12 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

/*
 *enable or disable debug mode
 *enabled: 1 to enable, 0 to disable
 */

void	gc_set_debug(t_gc_context *contex, int enabled)
{
	if (!contex)
		return ;
	contex->debug_mode = enabled;
}
