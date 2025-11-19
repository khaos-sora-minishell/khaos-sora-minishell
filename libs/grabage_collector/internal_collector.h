/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_collector.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 21:39:34 by akivam            #+#    #+#             */
/*   Updated: 2025/11/17 21:39:35 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_COLLECTOR_H
# define INTERNAL_COLLECTOR_H

# include "garbage_collector.h"
# include <stdlib.h>

t_collecter	**get_gc_head(void);
void		**get_gc_stack_start(void);

void		gc_collect(void);

// core GC logic functions
void		gc_mark(void);
void		gc_sweep(void);
t_collecter	*get_header_from_ptr(void *ptr);

// gc_mark helper functions
int			is_valid_pointer(void *ptr);
void		mark_memory_region(void *start, size_t size);
void		mark_pointer(void *ptr);
void		mark_from_stack(void);

// memory manipulation functions
void		*gc_memcpy(void *dest, const void *src, size_t n);
int			gc_memcmp(const void *s1, const void *s2, size_t n);
void		*gc_memchr(const void *s1, int c, size_t n);
void		*gc_memmove(void *dest, const void *src, size_t n);
void		*gc_memset(void *s, int c, size_t n);

#endif
