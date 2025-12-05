/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 17:30:39 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:25:14 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

void	collector_init(void *stack_start)
{
	void	**stack_ptr;

	if (!stack_start)
		return ;
	stack_ptr = get_gc_stack_start();
	if (!stack_ptr)
		return ;
	*stack_ptr = stack_start;
}
