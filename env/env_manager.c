/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 12:33:24 by akivam            #+#    #+#             */
/*   Updated: 2025/12/15 20:56:55 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "utils.h"

/*
 * tabloyu başlatır ve envp dizisinden verileri çeker
 */
t_env_table	*initilaze_env_table(char **envp, void *arena)
{
	t_env_table		*table;
	t_gc_context	*contex;
	int				i;
	char			*eq_pos;
	char			*key;
	char			*val;

	contex = (t_gc_context *)arena;
	table = gc_malloc(contex, sizeof(t_env_table));
	table->buckets = gc_calloc(contex, ENV_TABLE_SIZE, sizeof(t_env_bucket *));
	table->count = 0;
	i = 0;
	while (envp && envp[i])
	{
		eq_pos = ft_strchr(envp[i], '=');
		if (eq_pos)
		{
			key = gc_strndup(contex, envp[i], eq_pos - envp[i]);
			val = eq_pos + 1;
			env_set(table, key, val, arena);
		}
		i++;
	}
	return (table);
}

/*
** Değer Getir (Decryption ile)
*/
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

/*
** Değer Ata/Güncelle (Encryption + _has_value yönetimi)
*/
void	env_set(t_env_table *table, char *key, char *value, void *arena)
{
	unsigned long	idx;
	t_env_bucket	*current;
	t_env_bucket	*new_node;
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
            if (value)
            {
			    current->value = gc_strdup(contex, value);
			    xor_cipher(current->value);
                current->_has_value = 1;
            }
			return ;
		}
		current = current->next;
	}

    // 2. Yeni Ekleme Durumu
	new_node = gc_malloc(contex, sizeof(t_env_bucket));
	new_node->key = gc_strdup(contex, key);
	if (value)
    {
		new_node->value = gc_strdup(contex, value);
        new_node->_has_value = 1; // [YENİ]
    }
	else
    {
		new_node->value = gc_strdup(contex, "");
        new_node->_has_value = 0;
    }
	xor_cipher(new_node->value);
	new_node->next = table->buckets[idx];
	table->buckets[idx] = new_node;
	table->count++;
}

/*
** Değişken Silme (Linked List Bağlantı Hatası Giderildi)
*/
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

/*
** Tabloyu env dizisine çevir (Sadece _has_value == 1 olanlar)
*/
char	**env_table_to_array(t_env_table *table, void *arena)
{
	char			**arr;
	t_env_bucket	*curr;
	t_gc_context	*contex;
	int				i;
	int				k;
	char			*tmp_val;
	char			*tmp_join;

	contex = (t_gc_context *)arena;
	arr = gc_malloc(contex, sizeof(char *) * (table->count + 1));
	i = 0;
	k = 0;
	while (i < ENV_TABLE_SIZE)
	{
		curr = table->buckets[i];
		while (curr)
		{
            if (curr->_has_value == 1)
            {
			    tmp_val = gc_strdup(contex, curr->value);
			    xor_cipher(tmp_val);
			    tmp_join = gc_strjoin(contex, curr->key, "=");
			    arr[k++] = gc_strjoin(contex, tmp_join, tmp_val);
            }
			curr = curr->next;
		}
		i++;
	}
	arr[k] = NULL;
	return (arr);
}
