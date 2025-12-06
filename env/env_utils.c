/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:23 by akivam            #+#    #+#             */
/*   Updated: 2025/11/19 19:20:24 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"

/*
 * Get environment variable value
 */
char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/*
 * Set environment variable
 */
void	set_env_value(t_env **env, char *key, char *value, void *arena)
{
	t_env			*curr;
	t_env			*new_node;
	t_gc_context	*contex;

	contex = gc_get_current();
	curr = *env;
	while ((curr))
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			curr->value = gc_strdup(contex, value);
			return ;
		}
		curr = curr->next;
	}
	new_node = gc_malloc(contex, sizeof(t_env));
	new_node->key = gc_strdup(contex, key);
	new_node->value = gc_strdup(contex, value);
	new_node->next = *env;
	*env = new_node;
}

/*
 * Unset environment variable
 */
void	unset_env_value(t_env **env, char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

char	**env_list_to_array(t_env *env, void *arena)
{
	t_env			*tmp;
	char			**array;
	int				count;
	int				i;
	t_gc_context	*contex;
	char			*temp_str;

	contex = gc_get_current();
	count = 0;
	tmp = env;
	while (tmp && ++count)
		tmp = tmp->next;
	array = gc_malloc(contex, sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	i = 0;
	tmp = env;
	while (tmp)
	{
		if (tmp->value)
		{
			temp_str = gc_strjoin(contex, tmp->key, "=");
			array[i++] = gc_strjoin(contex, temp_str, tmp->value);
		}
		tmp = tmp->next;
	}
	array[i] = NULL;
	return (array);
}
