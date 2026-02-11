/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 12:06:34 by akivam            #+#    #+#             */
/*   Updated: 2026/02/12 02:43:46 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "garbage_collector.h"
#include "lexer.h"
#include "minishell.h"
#include <readline/history.h>

void	cleanup_child_process(t_shell *shell)
{
	rl_clear_history();
	get_next_line(-1);
	if (shell->current_input)
	{
		free(shell->current_input);
		shell->current_input = NULL;
	}
	if (shell->stdin_backup > 2)
		close(shell->stdin_backup);
	if (shell->stdout_backup > 2)
		close(shell->stdout_backup);
	if (shell->redir_stdin_backup > 2)
		close(shell->redir_stdin_backup);
	if (shell->redir_stdout_backup > 2)
		close(shell->redir_stdout_backup);
	if (shell->global_arena)
		gc_destroy(shell->global_arena);
	if (shell->cmd_arena)
		gc_destroy(shell->cmd_arena);
}

/*
 * Cleans up shell resources before exit
 * Saves history file and performs final cleanup
 */
void	cleanup_shell(t_shell *shell)
{
	save_history_file(shell);
	cleanup_child_process(shell);
}

/*
 * Splits input on newlines and processes each line separately
 * Handles pasted multi-line input correctly
 */
static void	process_multiline_input(t_shell *shell, char *input)
{
	char	*line_start;
	char	*line_end;
	char	*line;
	int		len;

	line_start = input;
	while (*line_start)
	{
		line_end = line_start;
		while (*line_end && *line_end != '\n')
			line_end++;
		len = line_end - line_start;
		line = gc_strndup(shell->cmd_arena, line_start, len);
		if (line && *line)
		{
			shell->current_input = line;
			process_input(shell, line);
			shell->current_input = NULL;
			clean_loop(shell);
		}
		if (*line_end == '\n')
			line_end++;
		line_start = line_end;
	}
}

/*
 * Processes user input through lexer, parser, and executor
 * Adds non-empty input to command history
 */
void	process_input(t_shell *shell, char *input)
{
	t_token	*tokens;

	if (*input)
		add_history_entry(shell, input);
	tokens = lexer(input, shell);
	if (tokens)
	{
		shell->ast_root = parser(tokens, shell);
		if (shell->ast_root)
		{
			shell->cmd_count++;
			executor_run(shell);
		}
	}
}

/*
 * Main interactive shell loop
 * Reads and processes commands until EOF or exit
 */
void	run_shell_loop(t_shell *shell)
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
		if (get_signal() == SIGINT)
			shell->exit_status = 130;
		if (ft_strchr(input, '\n'))
			process_multiline_input(shell, input);
		else
		{
			shell->current_input = input;
			process_input(shell, input);
			shell->current_input = NULL;
			clean_loop(shell);
		}
		free(input);
	}
}
