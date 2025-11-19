/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_header_from_ptr.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 20:04:00 by akivam            #+#    #+#             */
/*   Updated: 2025/11/17 20:04:03 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"

t_collecter	*get_header_from_ptr(void *ptr)
{
	t_collecter	*header;

	if (!ptr)
		return (NULL);
	header = (t_collecter *)ptr - 1;
	return (header);
}
