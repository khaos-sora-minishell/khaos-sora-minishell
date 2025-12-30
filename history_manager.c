/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 18:16:43 by akivam            #+#    #+#             */
/*   Updated: 2025/12/20 18:21:52 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_history_path(t_shell *shell)
{
	char	*home;
	char	*path;

	home = getenv("HOME");
	if (!home)
		return (NULL);
	path = gc_strjoin(shell->global_arena, home, "/.shell_history");
	return (path);
}

void	init_history(t_shell *shell)
{
	shell->history_file = get_history_path(shell);
	if (shell->history_file)
		read_history(shell->history_file);
}

void	save_history_file(t_shell *shell)
{
	if (shell->history_file)
	{
		write_history(shell->history_file);
	}
}
