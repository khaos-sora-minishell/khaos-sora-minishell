/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_mark_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 21:01:37 by harici            #+#    #+#             */
/*   Updated: 2025/11/28 20:25:14 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

static void	scan_stack_range(void *start, void *end)
{
	void	**current;

	current = (void **)start;
	while (current < (void **)end)
	{
		if (is_valid_pointer(*current))
			mark_pointer(*current);
		current++;
	}
}

//!/* Her iki yönü de desteklenebilir belki*/
void	mark_from_stack(void)
{
	void	**stack_start_ptr;
	void	*stack_start;
	void	*stack_end;
	int		stack_var;
	void	*temp;

	stack_start_ptr = get_gc_stack_start();
	stack_start = *stack_start_ptr;
	stack_end = &stack_var;
	if (stack_start > stack_end)
	{
		temp = stack_start;
		stack_start = stack_end;
		stack_end = temp;
	}
	scan_stack_range(stack_start, stack_end);
}
