/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:06:21 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 01:48:12 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "garbage_collector.h"
#include "lexer.h"
#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>

/*
 * Opens script file for reading in file mode
 * Sets exit status to 127 if file cannot be opened
 */
static int	open_script_file(t_shell *shell, char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_err_printf("minishell: %s: %s\n", filename, strerror(errno));
		shell->exit_status = 127;
	}
	return (fd);
}

/*
 * Reads and processes each line from script file
 * Skips shebang lines starting with #!
 */
static void	process_script_lines(t_shell *shell, int fd)
{
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] == '#' && line[1] == '!')
		{
			free(line);
			continue ;
		}
		shell->current_input = line;
		process_input(shell, line);
		free(line);
		shell->current_input = NULL;
		clean_loop(shell);
	}
	get_next_line(-1);
}

/*
 * Executes minishell in script file mode
 * Processes commands from file line by line
 */
static void	run_file_mode(t_shell *shell, char *filename)
{
	int	fd;

	fd = open_script_file(shell, filename);
	if (fd < 0)
		return ;
	process_script_lines(shell, fd);
	close(fd);
}

/*
 * Main entry point with bonus features
 * Supports interactive mode and script file execution with history
 */
int	main(int argc, char const *argv[], char **envp)
{
	t_shell	shell;

	init_shell(&shell, argc, (char **)argv, (char **)envp);
	shell.path_dirs = parse_path(&shell);
	load_shellrc(&shell);
	setup_signals();
	if (argc > 1)
		run_file_mode(&shell, (char *)argv[1]);
	else if (!isatty(STDIN_FILENO) || isatty(STDOUT_FILENO))
		run_shell_loop(&shell);
	else if (isatty(STDIN_FILENO))
		ft_printf("exit\n");
	clean_loop(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
}
