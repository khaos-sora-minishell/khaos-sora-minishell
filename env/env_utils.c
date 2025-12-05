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

/*
 * Get environment variable value
 */
char	*get_env_value(t_env *env, char *key)
{
	(void)env;
	(void)key;
	// TODO: Implement get env value
	return (NULL);
}

/*
 * Set environment variable
 */
void	set_env_value(t_env **env, char *key, char *value, void *arena)
{
	(void)env;
	(void)key;
	(void)value;
	(void)arena;
	// TODO: Implement set env value
}

/*
 * Unset environment variable
 */
void	unset_env_value(t_env **env, char *key)
{
	(void)env;
	(void)key;
	// TODO: Implement unset env value
}
