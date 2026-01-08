/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_collector.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 19:32:59 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 19:32:59 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_COLLECTOR_H
# define INTERNAL_COLLECTOR_H

/* Hash table configuration */
# define GC_HASH_SIZE 1024

/* FNV-1a hash constants */
# define FNV_OFFSET_BASIS 2166136261u
# define FNV_PRIME 16777619u
# define BYTE_MASK 0xFF

/* Memory pool configuration */
# define GC_POOL_SIZE 1048576
# define GC_SMALL_ALLOC_THRESHOLD 256

# include "../garbage_collector.h"

/*allocation metadata*/
typedef struct s_gc_allocation
{
	void					*ptr;
	size_t					size;
	size_t					scope_level;
	int						marked;
	int						from_pool;
	struct s_gc_allocation	*next;
	struct s_gc_allocation	*prev;
	struct s_gc_allocation	*scope_next;

}							t_gc_allocation;

/* Hash table bucket structure */
typedef struct s_gc_hash_bucket
{
	void					*ptr;
	t_gc_allocation			*allocation;
	struct s_gc_hash_bucket	*next;
}							t_gc_hash_bucket;

/* Memory pool structure */
typedef struct s_gc_pool
{
	void					*memory;
	size_t					size;
	size_t					used;
	struct s_gc_pool		*next;
}							t_gc_pool;

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
struct s_gc_context
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
	t_gc_hash_bucket		*hash_table[GC_HASH_SIZE];
	t_gc_pool				*pool_list;
	size_t					pool_count;
};

/*memory utility functions*/

void			*gc_memcpy(void *dest, const void *src, size_t n);
int				gc_memcmp(const void *s1, const void *s2, size_t n);
void			*gc_memchr(const void *s1, int c, size_t n);
void			*gc_memmove(void *dest, const void *src, size_t n);
void			*gc_memset(void *s, int c, size_t n);
void			gc_bzero(void *s, size_t n);

/*malloc utils*/
t_gc_allocation	*gc_meta_create(void *ptr, size_t size, size_t level,
					int from_pool);
void			gc_meta_add_global(t_gc_context *contex,
					t_gc_allocation *meta_data);
void			gc_meta_add_scope(t_gc_context *contex,
					t_gc_allocation *meta_data);
void			gc_update_and_collecte(t_gc_context *contex,
					size_t size);

/* internal helper funcitons*/
t_gc_allocation	*gc_alloc_crate(void *ptr, size_t size, size_t level);
t_gc_allocation	*gc_find_allocation(t_gc_context *contex, void *ptr);
void			gc_alloc_destroy(t_gc_allocation *allo);
void			gc_free(t_gc_context *contex, void *ptr);
void			gc_alloc_add_to_list(t_gc_context *contex,
					t_gc_allocation *allo);
void			gc_alloc_remove_from_list(t_gc_context *contex,
					t_gc_allocation *alloc);
void			gc_alloc_add_to_scope(t_gc_scope *scope,
					t_gc_allocation *alloc);

t_gc_scope		*gc_scope_create(size_t level);
void			gc_mark(t_gc_context *contex);
void			gc_sweep(t_gc_context *contex);
int				gc_scope_push(t_gc_context *contex);
void			gc_scope_destroy(t_gc_scope *scope);
void			gc_mark_pahese(t_gc_context *contex);
size_t			gc_sweep_phase(t_gc_context *contex);

/* gc_track_utils helper functions */
size_t			gc_estimate_size(void *ptr);
t_gc_allocation	*gc_create_meta(void *ptr, size_t size, size_t level);
void			gc_update_stats(t_gc_context *contex, size_t size);
void			gc_add_to_global(t_gc_context *contex, t_gc_allocation *meta);

/* Hash functions */
size_t			gc_hash_ptr(void *ptr);
void			gc_hash_add(t_gc_context *contex, void *ptr,
					t_gc_allocation *alloc);
t_gc_allocation	*gc_hash_find(t_gc_context *contex, void *ptr);
void			gc_hash_remove(t_gc_context *contex, void *ptr);
void			gc_hash_clear(t_gc_context *contex);

/* Pool functions */
t_gc_pool		*gc_pool_create(size_t size);
void			*gc_pool_alloc(t_gc_context *contex, size_t size);
void			gc_pool_destroy_all(t_gc_context *contex);

#endif
