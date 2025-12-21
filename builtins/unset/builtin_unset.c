/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:19:58 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 19:42:57 by akivam           ###   ########.fr       */
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
		env_unset(shell->env_table, args[i]);
		i++;
	}
	shell->env_array = env_table_to_array(shell->env_table, contex);
	shell->path_dirs = parse_path(shell);
	return (0);
}
