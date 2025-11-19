/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 17:30:39 by akivam            #+#    #+#             */
/*   Updated: 2025/11/17 21:03:10 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

void	collector_init(void *stack_start)
{
	void	**stack_ptr;

	stack_ptr = get_gc_stack_start();
	*stack_ptr = stack_start;
}
