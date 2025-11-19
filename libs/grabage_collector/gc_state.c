/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_state.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 17:12:56 by akivam            #+#    #+#             */
/*   Updated: 2025/11/17 20:43:31 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

t_collecter	**get_gc_head(void)
{
	static t_collecter	*head = NULL;

	return (&head);
}

void	**get_gc_stack_start(void)
{
	static void	*stack_start = NULL;

	return (&stack_start);
}
