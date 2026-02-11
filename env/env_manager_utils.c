/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 20:32:41 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 12:34:04 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include "garbage_collector.h"
#include "utils.h"

void	set_default_prompt_vars(t_env_table *table, void *arena)
{
	char	*terminal_name;
	char	*ps1;

	if (!env_get(table, "PS1", arena))
	{
		terminal_name = get_default_terminal_name(arena);
		ps1 = gc_strjoin(arena, terminal_name, "$ ");
		env_set(table, "PS1", ps1, arena);
	}
	if (!env_get(table, "PS2", arena))
		env_set(table, "PS2", "> ", arena);
}

void	mark_env_as_exported(t_env_table *table, char *key)
{
	unsigned long	idx;
	t_env_bucket	*current;

	idx = fnv1a_hash(key);
	current = table->buckets[idx];
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current->_is_exported = 1;
			break ;
		}
		current = current->next;
	}
}

static void	set_shlvl_var(t_env_table *table, void *arena)
{
	char	*shlvl_str;
	char	*new_shlvl;
	int		lvl;

	shlvl_str = env_get(table, "SHLVL", arena);
	if (!shlvl_str)
		lvl = 1;
	else
		lvl = ft_atoi(shlvl_str) + 1;
	new_shlvl = ft_itoa(lvl);
	if (new_shlvl)
	{
		gc_track((t_gc_context *)arena, new_shlvl);
		env_set(table, "SHLVL", new_shlvl, arena);
		mark_env_as_exported(table, "SHLVL");
	}
}

static void	set_pwd_var(t_env_table *table, void *arena)
{
	char	*cwd;

	if (!env_get(table, "PWD", arena))
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			gc_track((t_gc_context *)arena, cwd);
			env_set(table, "PWD", cwd, arena);
			mark_env_as_exported(table, "PWD");
		}
	}
}

void	set_default_env_vars(t_env_table *table, void *arena)
{
	set_shlvl_var(table, arena);
	set_pwd_var(table, arena);
}
