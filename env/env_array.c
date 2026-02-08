/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 01:38:56 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 01:39:02 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stddef.h>

static int	count_env_nodes(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static void	fill_env_array(char **array, t_env *env, t_gc_context *contex)
{
	int		i;
	char	*temp_str;

	i = 0;
	while (env)
	{
		if (env->value)
		{
			temp_str = gc_strjoin(contex, env->key, "=");
			array[i++] = gc_strjoin(contex, temp_str, env->value);
		}
		env = env->next;
	}
	array[i] = NULL;
}

char	**env_list_to_array(t_env *env, void *arena)
{
	t_gc_context	*contex;
	char			**array;
	int				count;

	contex = (t_gc_context *)arena;
	count = count_env_nodes(env);
	array = gc_malloc(contex, sizeof(char *) * (count + 1));
	if (!array)
		return (NULL);
	fill_env_array(array, env, contex);
	return (array);
}
