/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 21:08:00 by akivam            #+#    #+#             */
/*   Updated: 2025/12/01 14:15:49 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H

# include <stddef.h>

/*boolean values*/
# define GC_TRUE 1
# define GC_FALSE 0

/* return codes*/
# define GC_SUCCESS 0
# define GC_ERROR -1
# define GC_NULL_PTR -2

# define MAX_GC_CONTEXTS 16

void			*__real_malloc(size_t size);
void			*__real_calloc(size_t nmemb, size_t size);
void			*__real_realloc(void *ptr, size_t size);
void			__real_free(void *ptr);

/*opaque contex type*/

typedef struct s_gc_context	t_gc_context;

/*garbage collector modes */
typedef enum e_gc_mode
{
	GC_MODE_MANUAL = 0,
	GC_MODE_AUTO = 1,
	GC_MODE_HYBRID = 2
}	t_gc_mode;

/*statistic structure*/
typedef struct s_gc_stats
{
	size_t	total_allocated;
	size_t	total_freed;
	size_t	current_usage;
	size_t	peak_usage;
	size_t	allocation_count;
	size_t	scope_depth;
	size_t	free_count;
}			t_gc_stats;

/*initilazition*/
t_gc_context	*gc_create(void);
void			gc_destroy(t_gc_context *context);
/*memory allocation*/
void			*gc_malloc(t_gc_context *contex, size_t size);
void			*gc_calloc(t_gc_context *contex, size_t nmumb, size_t size);
void			*gc_realloc(t_gc_context *contex, void *ptr, size_t size);
/*string utilities*/
char			*gc_strdup(t_gc_context *contex, const char *s);
char			*gc_strndup(t_gc_context *contex, const char *s, size_t n);
char			*gc_strjoin(t_gc_context *contex, const char *s1,
					const char *s2);
size_t			gc_strlen(const char *s);
int				gc_strcmp(const char *s1, const char *s2);
int				gc_strncmp(const char *s1, const char *s2, size_t n);
char			*gc_strchr(const char *s, int c);
char			*gc_strrchr(const char *s, int c);
/*string conversion functions*/
int				gc_atoi(const char *str);
char			*gc_itoa(t_gc_context *contex, int n);
char			*gc_uitoa(t_gc_context *contex, size_t n);
/*string manipulation with gc tracking*/
char			*gc_substr(t_gc_context *contex, const char *s, size_t start,
					size_t len);
char			*gc_strtrim(t_gc_context *contex, const char *s1,
					const char *set);
char			**gc_split(t_gc_context *contex, const char *s, char c);
/*scope managment*/
int				gc_scope_push(t_gc_context *contex);
void			gc_scope_pop(t_gc_context *contex);
void			gc_scope_pop_all(t_gc_context *contex);
/*configuration*/
void			gc_set_mode(t_gc_context *contex, t_gc_mode mode);
t_gc_mode		gc_get_mode(t_gc_context *contex);
void			gc_set_debug(t_gc_context *contex, int enable);
/*statistic & debug*/
void			gc_get_stats(t_gc_context *contex, t_gc_stats *stats);
void			gc_print_stats(t_gc_context *contex);
void			gc_collect(t_gc_context *contex);
/*wrapper helper function*/
t_gc_context	*gc_get_current(void);
#endif
