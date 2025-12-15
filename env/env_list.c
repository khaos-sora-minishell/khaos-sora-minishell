/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:23 by akivam            #+#    #+#             */
/*   Updated: 2025/12/15 21:36:13 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <stddef.h>

static t_env	*create_env_node(char *key, char *value, void *arena)
{
	t_env	*node;

	node = gc_malloc((t_gc_context *)arena, sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

static void	env_add_back(t_env **head, t_env *new_node)
{
	t_env	*temp;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
}

static void	process_env_str(t_env **head, char *env_str, void *arena)
{
	char			*eq_pos;
	char			*key;
	char			*value;
	t_env			*node;
	t_gc_context	*garbage_collector_contex;

	garbage_collector_contex = (t_gc_context *)arena;
	eq_pos = ft_strchr(env_str, '=');
	if (eq_pos)
	{
		key = gc_strndup(garbage_collector_contex, env_str, eq_pos - env_str);
		value = gc_strdup(garbage_collector_contex, eq_pos + 1);
	}
	else
	{
		key = gc_strdup(garbage_collector_contex, env_str);
		value = NULL;
	}
	node = create_env_node(key, value, arena);
	if (node)
		env_add_back(head, node);
}

static void	setup_min_env(t_env **head, t_gc_context *garbage_collector_contex)
{
	char	*cwd;

	if (!get_env_value(*head, "PWD"))
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			set_env_value(head, "PWD", cwd, garbage_collector_contex);
			free(cwd);
		}
	}
	if (!get_env_value(*head, "SHLVL"))
		set_env_value(head, "SHLVL", "1", garbage_collector_contex);
	if (!get_env_value(*head, "_"))
		set_env_value(head, "_", "/usr/bin/env", garbage_collector_contex);
}

t_env	*init_env(char **envp, void *arena)
{
	t_env	*head;
	int		i;

	head = NULL;
	i = 0;
	if (envp)
	{
		while (envp[i])
		{
			process_env_str(&head, envp[i], arena);
			i++;
		}
	}
	setup_min_env(&head, (t_gc_context *)arena);
	return (head);
}
