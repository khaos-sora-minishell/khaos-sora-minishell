/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_collector.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 21:18:07 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 19:56:35 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_COLLECTOR_H
# define INTERNAL_COLLECTOR_H

# include "../garbage_collector.h"

/*allocation metadata*/
typedef struct s_gc_allocation
{
	void					*ptr;
	size_t					size;
	size_t					scope_level;
	int						marked;
	struct s_gc_allocation	*next;
	struct s_gc_allocation	*prev;
	struct s_gc_allocation	*scope_next;

}							t_gc_allocation;

/*scope node*/

typedef struct s_gc_scope
{
	size_t					level;
	t_gc_allocation			*first;
	t_gc_allocation			*last;
	size_t					allocation_count;
	struct s_gc_scope		*prev;
}							t_gc_scope;

/*main context*/
struct						s_gc_context
{
	t_gc_allocation			*all_allocations;
	t_gc_allocation			*all_allocations_tail;
	t_gc_scope				*current_scope;
	size_t					scope_depth;
	size_t					total_allocated;
	size_t					total_freed;
	size_t					current_usage;
	size_t					peak_usage;
	size_t					allocation_count;
	size_t					free_count;
	t_gc_mode				mode;
	int						debug_mode;
	int						auto_collect;
	size_t					collect_threshold;
	size_t					collect_interval;
	size_t					last_collect_count;
};

/*memory utility functions*/

void						*gc_memcpy(void *dest, const void *src, size_t n);
int							gc_memcmp(const void *s1, const void *s2, size_t n);
void						*gc_memchr(const void *s1, int c, size_t n);
void						*gc_memmove(void *dest, const void *src, size_t n);
void						*gc_memset(void *s, int c, size_t n);
void						gc_bzero(void *s, size_t n);

/* internal helper funcitons*/

t_gc_allocation				*gc_alloc_crate(void *ptr, size_t size,
								size_t level);
t_gc_allocation				*gc_find_allocation(t_gc_context *contex,
								void *ptr);
void						gc_alloc_destroy(t_gc_allocation *allo);
void						gc_free(t_gc_context *contex, void *ptr);
void						gc_alloc_add_to_list(t_gc_context *contex,
								t_gc_allocation *allo);
void						gc_alloc_remove_from_list(t_gc_context *contex,
								t_gc_allocation *alloc);
void						gc_alloc_add_to_scope(t_gc_scope *scope,
								t_gc_allocation *alloc);

t_gc_scope					*gc_scope_create(size_t level);
void						gc_mark(t_gc_context *contex);
void						gc_sweep(t_gc_context *contex);
int							gc_scope_push(t_gc_context *contex);
void						gc_scope_destroy(t_gc_scope *scope);
void						gc_mark_pahese(t_gc_context *contex);
size_t						gc_sweep_phase(t_gc_context *contex);

#endif
