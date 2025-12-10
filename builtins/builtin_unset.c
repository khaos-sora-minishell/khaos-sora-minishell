/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:58 by akivam            #+#    #+#             */
/*   Updated: 2025/12/10 17:26:56 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(char **args, t_shell *shell)
{
	int				i;
	t_gc_context	*contex;

	i = 1;
	contex = (t_gc_context *)shell->global_arena;
	while (args[i])
	{
		unset_env_value(&shell->env_list, args[i]);
		i++;
	}
	// Update env_array and path_dirs after unset
	shell->env_array = env_list_to_array(shell->env_list, contex);
	shell->path_dirs = parse_path(shell->env_list, contex);
	return (0);
}
