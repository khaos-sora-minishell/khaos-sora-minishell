/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 12:33:24 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 20:34:28 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "utils.h"

static void	update_env_node(t_env_bucket *node, char *value,
		t_gc_context *contex)
{
	if (value)
	{
		node->value = gc_strdup(contex, value);
		xor_cipher(node->value);
		node->_has_value = 1;
	}
}

static t_env_bucket	*create_env_node(char *key, char *value,
		t_gc_context *contex)
{
	t_env_bucket	*node;

	node = gc_malloc(contex, sizeof(t_env_bucket));
	node->key = gc_strdup(contex, key);
	if (value)
	{
		node->value = gc_strdup(contex, value);
		node->_has_value = 1;
	}
	else
	{
		node->value = gc_strdup(contex, "");
		node->_has_value = 0;
	}
	xor_cipher(node->value);
	node->next = NULL;
	return (node);
}

char	*env_get(t_env_table *table, char *key, void *arena)
{
	unsigned long	idx;
	t_env_bucket	*current;
	char			*decrypted_val;

	if (!table || !key)
		return (NULL);
	idx = fnv1a_hash(key);
	current = table->buckets[idx];
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (current->_has_value == 0)
				return (NULL);
			decrypted_val = gc_strdup((t_gc_context *)arena, current->value);
			xor_cipher(decrypted_val);
			return (decrypted_val);
		}
		current = current->next;
	}
	return (NULL);
}

void	env_set(t_env_table *table, char *key, char *value, void *arena)
{
	unsigned long	idx;
	t_env_bucket	*current;
	t_gc_context	*contex;

	if (!table || !key)
		return ;
	contex = (t_gc_context *)arena;
	idx = fnv1a_hash(key);
	current = table->buckets[idx];
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			update_env_node(current, value, contex);
			return ;
		}
		current = current->next;
	}
	current = create_env_node(key, value, contex);
	current->next = table->buckets[idx];
	table->buckets[idx] = current;
	table->count++;
}

void	env_unset(t_env_table *table, char *key)
{
	unsigned long	idx;
	t_env_bucket	*current;
	t_env_bucket	*prev;

	if (!table || !key)
		return ;
	idx = fnv1a_hash(key);
	current = table->buckets[idx];
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				table->buckets[idx] = current->next;
			table->count--;
			return ;
		}
		prev = current;
		current = current->next;
	}
}
