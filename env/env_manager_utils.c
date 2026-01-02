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

static void	set_default_prompt_vars(t_env_table *table, void *arena)
{
	if (!env_get(table, "PS1", arena))
		env_set(table, "PS1", "minishell$ ", arena);
	if (!env_get(table, "PS2", arena))
		env_set(table, "PS2", "> ", arena);
}

static void	set_default_env_vars(t_env_table *table, void *arena)
{
	char	*shlvl_str;
	char	*new_shlvl;
	int		lvl;
	char	*cwd;

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
	}
	if (!env_get(table, "PWD", arena))
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			gc_track((t_gc_context *)arena, cwd);
			env_set(table, "PWD", cwd, arena);
		}
	}
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
