/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_hash.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 19:53:22 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 19:53:22 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_collector.h"
#include <stdlib.h>

/*
		FNV-1a hash function for pointer addresses
		reurnns hash index in range [0, GC_HASH_SIZE]
*/
size_t	gc_hash_ptr(void *ptr)
{
	size_t	address;
	size_t	hash;
	int		i;

	address = (size_t)ptr;
	hash = FNV_OFFSET_BASIS;
	i = 0;
	while (i < (int) sizeof(void *))
	{
		hash ^= (address & BYTE_MASK);
		hash *= FNV_PRIME;
		address >>= 8;
		i++;
	}
	return (hash % GC_HASH_SIZE);
}

/*
	add allocation to hash table
	creates bucket and adds to font of chain (O(1))
*/

void	gc_hash_add(t_gc_context *contex, void *ptr,
		t_gc_allocation *allocation)
{
	t_gc_hash_bucket	*bucket;
	size_t				index;

	if (!contex || !ptr || !allocation)
		return ;
	bucket = (t_gc_hash_bucket *)malloc(sizeof(t_gc_hash_bucket));
	if (!bucket)
		return ;
	bucket->ptr = ptr;
	bucket->allocation = allocation;
	index = gc_hash_ptr(ptr);
	bucket->next = contex->hash_table[index];
	contex->hash_table[index] = bucket;
}

/*
	find allocation in hash table (O(1)) average case
	returns allocation meradata or NULL if not found
*/

t_gc_allocation	*gc_hash_find(t_gc_context *contex, void *ptr)
{
	t_gc_hash_bucket	*bucket;
	size_t				index;

	if (!contex || !ptr)
		return (NULL);
	index = gc_hash_ptr(ptr);
	bucket = contex->hash_table[index];
	while (bucket)
	{
		if (bucket->ptr == ptr)
			return (bucket->allocation);
		bucket = bucket->next;
	}
	return (NULL);
}

/*
	remove allocation from hash table
	frees the bucket but not the allocation itself
*/
void	gc_hash_remove(t_gc_context *contex, void *ptr)
{
	t_gc_hash_bucket	*bucket;
	t_gc_hash_bucket	*prev;
	size_t				index;

	if (!contex || !ptr)
		return ;
	index = gc_hash_ptr(ptr);
	bucket = contex->hash_table[index];
	prev = NULL;
	while (bucket)
	{
		if (bucket->ptr == ptr)
		{
			if (prev)
				prev->next = bucket->next;
			else
				contex->hash_table[index] = bucket->next;
			free(bucket);
			return ;
		}
		prev = bucket;
		bucket = bucket->next;
	}
}

/*
	cleare entire hash table
	frees all buvkets but not allocations
	called during gc_destroy()
*/

void	gc_hash_clear(t_gc_context *contex)
{
	t_gc_hash_bucket	*bucket;
	t_gc_hash_bucket	*next;
	size_t				i;

	if (!contex)
		return ;
	i = 0;
	while (i < GC_HASH_SIZE)
	{
		bucket = contex->hash_table[i];
		while (bucket)
		{
			next = bucket->next;
			free(bucket);
			bucket = next;
		}
		contex->hash_table[i] = NULL;
		i++;
	}
}
