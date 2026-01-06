/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_loader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 17:22:21 by akivam            #+#    #+#             */
/*   Updated: 2025/12/21 21:39:32 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#ifdef BONUS

static void	process_config_line(t_shell *shell, char *line)
{
	t_token	*tokens;
	char	*new_line;

	new_line = ft_strchr(line, '\n');
	if (new_line)
		*new_line = '\0';
	if (!*line || *line == '#')
		return ;
	tokens = lexer(line, shell);
	if (tokens)
	{
		shell->ast_root = parser(tokens, shell);
		if (shell->ast_root)
		{
			executor_run(shell);
		}
	}
	gc_scope_pop_all(shell->cmd_arena);
	shell->ast_root = NULL;
}

void	load_shellrc(t_shell *shell)
{
	int		fd;
	char	*path;
	char	*line;
	char	*home_dir;

	home_dir = env_get(shell->env_table, "HOME", shell->global_arena);
	if (!home_dir)
		return ;
	path = gc_strjoin(shell->global_arena, home_dir, "/.shellrc");
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return ;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		process_config_line(shell, line);
		free(line);
	}
	get_next_line(-1);
	close(fd);
}

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

#else

void	load_shellrc(t_shell *shell)
{
	(void)shell;
}

void	create_shellrc(t_shell *shell)
{
	(void)shell;
}

void	save_to_shellrc(t_shell *shell, char *line)
{
	(void)shell;
	(void)line;
}

#endif
