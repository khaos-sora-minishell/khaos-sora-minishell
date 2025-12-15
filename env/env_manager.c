/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 12:33:24 by akivam            #+#    #+#             */
/*   Updated: 2025/12/15 20:42:59 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/*
 *tabloyu başlatır ve envp (char **) dizisinden verileri çeker
 *linked-list tem hash table a geçiş burada yapılır
 */

t_env_table	*initilaze_env_table(char **envp, void *arena)
{
	t_env_table		*table;
	t_gc_context	*contex;
	int				i;
	char			*eq_pos;
	char			*ket;
	char			*val;

	contex = (t_gc_context *)arena;
	table = gc_malloc(contex, sizeof(t_env_table));
	table->buckets = gc_calloc(contex, ENV_TABLE_SIZE, sizeof(t_env_bucket *));
	table->count = 0;
	i = 0;
	while (envp && envp[i])
	{
		// key value ayrıştırması
		eq_pos = ft_strchr(envp[i], '=');
		if (eq_pos)
		{
			ket = gc_strndup(contex, envp[i], eq_pos - envp[i]);
			val = eq_pos + 1;
			env_set(table, key, val, arena);
		}
		i++;
	}
	return (table);
}

/*
** Değer Getir (Decryption ile)
** Şifreli veriyi çözer, temiz bir kopyasını döner.
*/
char	*env_get(t_env_table *table, char *key, void *arena)
{
	unsigned long	idx;
	t_env_bucket	*current;
	char			*decrypted_val;

	if (!table || !key)
		return (NULL);
	// fnv1a_hash fonksiyonunu env_crypto.c'den çağırır
	idx = fnv1a_hash(key);
	current = table->buckets[idx];
	while (current)
	{
		if (ft_strncmp(current->key, key) == 0)
		{
			// veri şifreli oldu için kopyasını alıp çözüme vermeliyiz
			decrypted_val = gc_strdup((t_gc_context *)arena, current->value);
			xor_cipher(decrypted_val);
			return (decrypted_val);
		}
		current = current->next;
	}
	return (NULL);
}

/*
** Değer Ata/Güncelle (Encryption ile)
** Veriyi tabloya kaydederken otomatik şifreler.
*/

void	env_set(t_env_table *table, char *key, char *value, void *arena)
{
	unsigned long	idx;
	t_env_bucket	*currrent;
	t_env_bucket	*new_node;
	t_gc_context	*contex;

	if (!table || !key)
		return ;
	contex = (t_gc_context *)arena;
	idx = fnv1a_hash(key);
	currrent = table->buckets[idx];
	while (currrent)
	{
		if (ft_strcmp(currrent->key, key) == 0)
		{
			currrent->value = gc_strdup(contex, value);
			xor_cipher(currrent->value);
			return ;
		}
		currrent = currrent->next;
	}
	new_node = gc_malloc(contex, sizeof(t_env_bucket));
	new_node->key = gc_strdup(contex, key);
	if (value)
		new_node->value = gc_strdup(contex, value);
	else
		new_node->value = gc_strdup(contex, "");
	xor_cipher(new_node->value);
	new_node->next = table->buckets[idx];
	table->buckets[idx] = new_node;
	table->count++;
}

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
			// Geçici kopya al ve çöz
			tmp_val = gc_strdup(contex, curr->value);
			xor_cipher(tmp_val);
			tmp_join = gc_strjoin(contex, curr->key, "=");
			arr[k++] = gc_strjoin(contex, tmp_join, tmp_val);
			curr = curr->next;
		}
		i++;
	}
	arr[k] = NULL;
	return (arr);
}
