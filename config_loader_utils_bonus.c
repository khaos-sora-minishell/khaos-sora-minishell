/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_loader_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 21:54:25 by akivam            #+#    #+#             */
/*   Updated: 2026/01/06 22:01:08 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_shellrc(t_shell *shell)
{
	int		fd;
	char	*path;
	char	*home_dir;

	home_dir = env_get(shell->env_table, "HOME", shell->global_arena);
	if (!home_dir)
		return ;
	path = gc_strjoin(shell->global_arena, home_dir, "/.shellrc");
	fd = open(path, O_CREAT | O_WRONLY | O_EXCL, 0644);
	if (fd >= 0)
	{
		write(fd, "# Minishell configuration file\n", 31);
		write(fd, "# Auto-generated - Edit carefully\n\n", 35);
		close(fd);
	}
}

void	save_to_shellrc(t_shell *shell, char *line)
{
	int		fd;
	char	*path;
	char	*home_dir;

	if (!line || !*line)
		return ;
	home_dir = env_get(shell->env_table, "HOME", shell->global_arena);
	if (!home_dir)
		return ;
	path = gc_strjoin(shell->global_arena, home_dir, "/.shellrc");
	fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd >= 0)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		close(fd);
	}
}
