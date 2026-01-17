/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_manager_utils_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 03:07:25 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 01:45:09 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <fcntl.h>
#include <readline/readline.h>

char	*get_history_path(t_shell *shell)
{
	char	*home;
	char	*path;

	home = getenv("HOME");
	if (!home)
		return (NULL);
	path = gc_strjoin(shell->global_arena, home, "/.minishell_history");
	return (path);
}

void	load_history_from_file(char *path)
{
	int		fd;
	char	*line;
	char	*trimmed;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return ;
	line = get_next_line(fd);
	while (line)
	{
		trimmed = ft_strtrim(line, "\n");
		if (trimmed && trimmed[0])
			add_history(trimmed);
		free(trimmed);
		free(line);
		line = get_next_line(fd);
	}
	get_next_line(-1);
	close(fd);
}

void	save_history_file(t_shell *shell)
{
	if (shell->history_fd > 0)
	{
		close(shell->history_fd);
		shell->history_fd = -1;
	}
}
