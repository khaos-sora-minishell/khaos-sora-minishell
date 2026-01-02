/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/02 17:08:38 by akivam            #+#    #+#             */
/*   Updated: 2026/01/02 18:24:28 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "garbage_collector.h"

void	set_default_shell_vars(t_env_table *table, void *arena)
{
	set_default_env_vars(table, arena);
	set_default_prompt_vars(table, arena);
}

t_env_table	*initialize_env_table(char **envp, void *arena)
{
	t_env_table		*table;
	t_gc_context	*contex;
	int				i;

	contex = (t_gc_context *)arena;
	table = gc_malloc(contex, sizeof(t_env_table));
	table->buckets = gc_calloc(contex, ENV_TABLE_SIZE, sizeof(t_env_bucket *));
	table->count = 0;
	i = 0;
	while (envp && envp[i])
	{
		add_env_entry(table, envp[i], contex);
		i++;
	}
	set_default_shell_vars(table, arena);
	return (table);
}

static char	*join_env_pair(t_env_bucket *node, t_gc_context *contex)
{
	char	*tmp_val;
	char	*tmp_join;
	char	*result;

	tmp_val = gc_strdup(contex, node->value);
	xor_cipher(tmp_val);
	tmp_join = gc_strjoin(contex, node->key, "=");
	result = gc_strjoin(contex, tmp_join, tmp_val);
	return (result);
}

char	**env_table_to_array(t_env_table *table, void *arena)
{
	char			**arr;
	t_env_bucket	*current;
	int				idx;
	int				k;

	arr = gc_malloc((t_gc_context *)arena, sizeof(char *) * (table->count + 1));
	idx = 0;
	k = 0;
	while (idx < ENV_TABLE_SIZE)
	{
		current = table->buckets[idx];
		while (current)
		{
			if (current->_has_value == 1)
				arr[k++] = join_env_pair(current, (t_gc_context *)arena);
			current = current->next;
		}
		idx++;
	}
	arr[k] = NULL;
	return (arr);
}
