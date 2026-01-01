/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 22:10:55 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 19:55:00 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbage_collector.h"
#include "minishell.h"

char	**parse_path(t_shell *shell)
{
	char	*path;

	if (!shell || !shell->env_table)
		return (NULL);
	path = env_get(shell->env_table, "PATH", shell->global_arena);
	if (!path)
		return (NULL);
	return (gc_split(shell->global_arena, path, ':'));
}
