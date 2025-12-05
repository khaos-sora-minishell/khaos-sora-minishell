/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_sweep.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:34:50 by harici            #+#    #+#             */
/*   Updated: 2025/11/28 20:25:14 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

static void	sweep_unmarked(t_collecter **head_ptr, t_collecter **prev,
		t_collecter *current, t_collecter *next)
{
	if (*prev)
		(*prev)->next = next;
	else
		*head_ptr = next;
	free(current);
}

void	gc_sweep(void)
{
	t_collecter	**head_ptr;
	t_collecter	*current;
	t_collecter	*prev;
	t_collecter	*next;

	head_ptr = get_gc_head();
	current = *head_ptr;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (!current->is_marked)
			sweep_unmarked(head_ptr, &prev, current, next);
		else
		{
			current->is_marked = 0;
			prev = current;
		}
		current = next;
	}
}
