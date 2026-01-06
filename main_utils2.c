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

void	cleanup_shell(t_shell *shell)
{
	save_history_file(shell);
	cleanup_child_process(shell);
}

#else

void	cleanup_shell(t_shell *shell)
{
	cleanup_child_process(shell);
}

#endif

void	process_input(t_shell *shell, char *input)
{
	t_token	*tokens;

	if (*input)
		add_history(input);
	tokens = lexer(input, shell);
	if (tokens)
	{
		shell->ast_root = parser(tokens, shell);
		if (shell->ast_root)
			executor_run(shell);
	}
}

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
