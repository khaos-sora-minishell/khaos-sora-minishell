/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 02:25:01 by akivam            #+#    #+#             */
/*   Updated: 2026/02/12 02:53:08 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "garbage_collector.h"
#include "lexer.h"
#include "libft.h"
#include "shell.h"
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>

static void	init_shell_common(t_shell *shell, int argc, char **argv,
		char **envp)
{
	ft_bzero(shell, sizeof(t_shell));
	shell->global_arena = gc_create();
	shell->cmd_arena = gc_create();
	if (!shell->global_arena || !shell->cmd_arena)
		exit(1);
	shell->env_table = initialize_env_table(envp, shell->global_arena);
	shell->env_array = env_table_to_array(shell->env_table,
			shell->global_arena);
	shell->alias_table = gc_calloc(shell->global_arena, 1, sizeof(t_env_table));
	shell->alias_table->buckets = gc_calloc(shell->global_arena,
			ENV_TABLE_SIZE, sizeof(t_env_bucket *));
	shell->terminal_name = get_default_terminal_name(shell->global_arena);
	shell->terminal_name_color = NULL;
	shell->terminal_bg_color = NULL;
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	shell->redir_stdin_backup = -1;
	shell->redir_stdout_backup = -1;
	shell->current_input = NULL;
	shell->argc = argc;
	shell->argv = argv;
	shell->shell_pid = read_shell_pid();
	shell->exit_status = 0;
	shell->cmd_count = 0;
}

void	init_shell(t_shell *shell, int argc, char **argv, char **envp)
{
	init_shell_common(shell, argc, argv, envp);
	init_history(shell);
	create_shellrc(shell);
}

void	clean_loop(t_shell *shell)
{
	gc_scope_pop_all(shell->cmd_arena);
	set_signal(0);
	shell->ast_root = NULL;
}
