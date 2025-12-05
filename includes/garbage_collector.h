/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 16:57:17 by akivam            #+#    #+#             */
/*   Updated: 2025/11/28 20:25:14 by akivam           ###   ########.tr       */
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
/**
 //! gc_calloc - Allocate and zero memory
 //! @nmemb: Number of elements
 //! @size: Size of each element
 //! 
 //! Note: Returns NULL if nmemb or size is 0.
 //! 
 //! Returns: Pointer to zeroed allocation, or NULL on failure
 **/
void					*gc_calloc(size_t nmemb, size_t size);

/**
 //! gc_realloc - Reallocate memory
 //! @ptr: Pointer to reallocate
 //! @size: New size
 //! 
 //! Note: If size is 0, returns NULL without immediately freeing ptr.
 //!       The old allocation will be collected during next GC cycle.
 //! 
 //! Returns: Pointer to new allocation, or NULL on failure
 */
void					*gc_realloc(void *ptr, size_t size);

#endif
