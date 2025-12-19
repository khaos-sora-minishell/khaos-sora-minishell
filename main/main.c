/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 16:49:52 by harici            #+#    #+#             */
/*   Updated: 2025/12/19 16:49:54 by harici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*line;

	(void)ac;
	(void)av;
	// TODO: Shell initialization
	// shell.global_arena = gc_create_arena();
	// shell.env_list = init_env(envp, shell.global_arena);
	// shell.exit_status = 0;
	// setup_signals();
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		// TODO: Process command
		free(line);
	}
	// TODO: Cleanup
	return (shell.exit_status);
}
