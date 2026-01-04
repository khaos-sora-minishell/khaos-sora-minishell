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

int	main(int argc, char const *argv[], char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, (char **)envp);
	shell.path_dirs = parse_path(&shell);
	load_shellrc(&shell);
	setup_signals();
	run_shell_loop(&shell);
	clean_loop(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
}
