/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:57:17 by akivam            #+#    #+#             */
/*   Updated: 2025/11/17 16:57:17 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H

# include <stddef.h>

typedef struct s_collecter
{
	unsigned char		is_marked;
	size_t				size;
	struct s_collecter	*next;

}						t_collecter;

void					collector_init(void *stack_start);
void					collector_close(void);
void					gc_collect(void);
void					*gc_malloc(size_t size);
void					*gc_calloc(size_t nmemb, size_t size);
void					*gc_realloc(void *ptr, size_t size);

#endif
