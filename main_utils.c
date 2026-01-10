/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: harici <harici@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 21:13:51 by harici            #+#    #+#             */
/*   Updated: 2026/01/10 18:48:15 by harici           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "executor.h"
#include "garbage_collector.h"
#include "lexer.h"
#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>

#ifdef BONUS

/*
 * Initializes shell structure and sets up memory arenas
 * Creates environment table and alias table with history support
 */
void	init_shell(t_shell *shell, int argc, char **argv, char **envp)
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
	shell->argc = argc;
	shell->argv = argv;
	init_history(shell);
	create_shellrc(shell);
	shell->exit_status = 0;
}

#else

/*
 * Initializes shell structure and sets up memory arenas
 * Creates environment table and alias table without history support
 */
void	init_shell(t_shell *shell, int argc, char **argv, char **envp)
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
	shell->argc = argc;
	shell->argv = argv;
	shell->exit_status = 0;
}

#endif

/*
 * Cleans up command arena after each command execution
 * Resets signal state and AST root for next command
 */
void	clean_loop(t_shell *shell)
{
	gc_scope_pop_all(shell->cmd_arena);
	set_signal(0);
	shell->ast_root = NULL;
}

/*
 * Performs cleanup before child process termination
 * Closes file descriptors and frees allocated memory
 */
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
