/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 02:00:00 by harici            #+#    #+#             */
/*   Updated: 2025/12/24 02:00:01 by harici           ###   ########.fr       */
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

static void	run_shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = read_multiline(shell);
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				ft_printf("exit\n");
			break ;
		}
		shell->current_input = input;
		process_input(shell, input);
		free(shell->current_input);
		shell->current_input = NULL;
		clean_loop(shell);
	}
}

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
}

static void	run_file_mode(t_shell *shell, char *filename)
{
	int	fd;

	fd = open_script_file(shell, filename);
	if (fd < 0)
		return ;
	process_script_lines(shell, fd);
	close(fd);
}

int	main(int argc, char const *argv[], char **envp)
{
	t_shell	shell;

	init_shell(&shell, argc, (char **)argv, (char **)envp);
	shell.path_dirs = parse_path(&shell);
	load_shellrc(&shell);
	setup_signals();
	if (argc > 1)
		run_file_mode(&shell, (char *)argv[1]);
	else
		run_shell_loop(&shell);
	clean_loop(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
}
