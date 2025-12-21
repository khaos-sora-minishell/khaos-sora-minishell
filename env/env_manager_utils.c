/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 20:32:41 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 20:34:12 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_default_shell_vars(t_env_table *table, void *arena)
{
	if (!env_get(table, "PS1", arena))
		env_set(table, "PS1", "minishell$ ", arena);
	if (!env_get(table, "PS2", arena))
		env_set(table, "PS2", "> ", arena);
}

static void	add_env_entry(t_env_table *table, char *env_str,
		t_gc_context *contex)
{
	char	*eq_pos;
	char	*key;

	eq_pos = ft_strchr(env_str, '=');
	if (eq_pos)
	{
		key = gc_strndup(contex, env_str, eq_pos - env_str);
		env_set(table, key, eq_pos + 1, contex);
	}
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
