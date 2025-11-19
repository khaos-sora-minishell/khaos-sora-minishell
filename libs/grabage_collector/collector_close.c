/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector_close.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:46:45 by harici            #+#    #+#             */
/*   Updated: 2025/11/17 20:46:59 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

void	collector_close(void)
{
	t_collecter	**head_ptr;
	t_collecter	*current;
	t_collecter	*next;

	head_ptr = get_gc_head();
	current = *head_ptr;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*head_ptr = NULL;
}
