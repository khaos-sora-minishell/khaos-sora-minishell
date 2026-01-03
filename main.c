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

void	init_shell(t_shell *shell, char **envp)
{
	ft_bzero(shell, sizeof(t_shell));
	shell->global_arena = gc_create();
	shell->cmd_arena = gc_create();
	if (!shell->global_arena || !shell->cmd_arena)
		exit(1);
	shell->env_table = initialize_env_table(envp, shell->global_arena);
	shell->env_array = env_table_to_array(shell->env_table,
			shell->global_arena);
	shell->alias_table = gc_calloc(shell->global_arena, 1,
			sizeof(t_env_table));
	shell->alias_table->buckets = gc_calloc(shell->global_arena,
			ENV_TABLE_SIZE, sizeof(t_env_bucket *));
	shell->terminal_name = "minishell";
	shell->terminal_name_color = NULL;
	shell->terminal_bg_color = NULL;
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	shell->redir_stdin_backup = -1;
	shell->redir_stdout_backup = -1;
	shell->current_input = NULL;
	init_history(shell);
	create_shellrc(shell);
	shell->exit_status = 0;
}

static void	clean_loop(t_shell *shell)
{
	gc_scope_pop_all(shell->cmd_arena);
	set_signal(0);
	shell->ast_root = NULL;
}

void	cleanup_shell(t_shell *shell)
{
	save_history_file(shell);
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

static void	process_input(t_shell *shell, char *input)
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

int	main(int argc, char const *argv[], char **envp)
{
	t_shell	shell;
	char	*input;

	(void)argc;
	(void)argv;
	init_shell(&shell, (char **)envp);
	shell.path_dirs = parse_path(&shell);
	load_shellrc(&shell);
	setup_signals();
	while (1)
	{
		input = read_multiline(&shell);
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				ft_printf("exit\n");
			break ;
		}
		shell.current_input = input;
		process_input(&shell, input);
		free(shell.current_input);
		shell.current_input = NULL;
		clean_loop(&shell);
	}
	clean_loop(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
}
