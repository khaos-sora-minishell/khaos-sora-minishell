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

#include "shell.h"
#include "utils.h"

static int	is_path_variable(char *arg)
{
	if (!arg)
		return (0);
	if (ft_strcmp(arg, "PATH") == 0)
		return (1);
	return (0);
}

int	builtin_unset(char **args, t_shell *shell)
{
	int				i;
	int				path_changed;
	t_gc_context	*contex;

	i = 1;
	path_changed = 0;
	contex = (t_gc_context *)shell->global_arena;
	while (args[i])
	{
		if (is_path_variable(args[i]))
			path_changed = 1;
		env_unset(shell->env_table, args[i]);
		i++;
	}
	shell->env_array = env_table_to_array(shell->env_table, contex);
	if (path_changed)
		shell->path_dirs = parse_path(shell);
	return (0);
}
