/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:23 by akivam            #+#    #+#             */
/*   Updated: 2025/11/19 19:20:23 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/* create new a env node*/
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

/*add element of list end  */
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

/* pars to "KEY=VALUE" string and add list*/
static void	process_env_str(t_env **head, char *env_str, void *arena)
{
	char			*eq_pos;
	char			*key;
	char			*value;
	t_env			*node;

	arena = gc_get_current(); // arena parameter maybe
	eq_pos = ft_strchr(env_str, '=');
	if (eq_pos)
	{
		key = gc_strndup(arena, env_str, eq_pos - env_str);
		value = gc_strdup(arena, eq_pos + 1);
	}
	else
	{
		key = gc_strdup(arena, env_str);
		value = NULL; // EXPORT edilmiş ama değeri yok
	}
	node = create_env_node(key, value, arena);
	if (node)
		env_add_back(head, node);
}

/*
 * Environment linked list operations
 */
t_env	*init_env(char **envp, void *arena)
{
	t_env	*head;
	int		i;

	head = NULL;
	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
	{
		process_env_str(&head, envp[i], arena);
		i++;
	}
	return (head);
}
