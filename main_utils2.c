/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 21:50:58 by akivam            #+#    #+#             */
/*   Updated: 2026/01/06 21:54:04 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"
#include "minishell.h"
#include <readline/history.h>

#ifdef BONUS

/*
 * Cleans up shell resources before exit
 * Saves history file and performs final cleanup
 */
void	cleanup_shell(t_shell *shell)
{
	save_history_file(shell);
	cleanup_child_process(shell);
}

#else

/*
 * Cleans up shell resources before exit
 * Performs final cleanup without history saving
 */
void	cleanup_shell(t_shell *shell)
{
	cleanup_child_process(shell);
}

#endif

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
			executor_run(shell);
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
		shell->current_input = input;
		process_input(shell, input);
		free(shell->current_input);
		shell->current_input = NULL;
		clean_loop(shell);
	}
}
