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

#include "shell.h"

static void	execute_config_cmd(t_shell *shell, char *line)
{
	t_token	*tokens;
	int		stderr_backup;

	stderr_backup = dup(STDERR_FILENO);
	close(STDERR_FILENO);
	tokens = lexer(line, shell);
	if (tokens)
	{
		shell->ast_root = parser(tokens, shell);
		if (shell->ast_root)
		{
			shell->cmd_count++;
			executor_run(shell);
		}
	}
	dup2(stderr_backup, STDERR_FILENO);
	close(stderr_backup);
}

static void	process_config_line(t_shell *shell, char *line)
{
	char	*new_line;

	new_line = ft_strchr(line, '\n');
	if (new_line)
		*new_line = '\0';
	if (!*line || *line == '#')
	{
		gc_scope_pop_all(shell->cmd_arena);
		shell->ast_root = NULL;
		return ;
	}
	execute_config_cmd(shell, line);
	gc_scope_pop_all(shell->cmd_arena);
	shell->ast_root = NULL;
}

static char	**read_shellrc_lines(t_shell *shell, int fd)
{
	char	**lines;
	char	*line;
	int		count;

	lines = gc_calloc(shell->global_arena, 256, sizeof(char *));
	count = 0;
	line = get_next_line(fd);
	while (line && count < 255)
	{
		lines[count++] = gc_strdup(shell->global_arena, line);
		free(line);
		line = get_next_line(fd);
	}
	get_next_line(-1);
	close(fd);
	return (lines);
}

void	load_shellrc(t_shell *shell)
{
	int		fd;
	char	*path;
	char	*home_dir;
	char	**lines;
	int		i;

	home_dir = env_get(shell->env_table, "HOME", shell->global_arena);
	if (!home_dir)
		return ;
	path = gc_strjoin(shell->global_arena, home_dir, "/.shellrc");
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		create_shellrc(shell);
		return ;
	}
	lines = read_shellrc_lines(shell, fd);
	i = -1;
	while (lines[++i])
		process_config_line(shell, lines[i]);
}
