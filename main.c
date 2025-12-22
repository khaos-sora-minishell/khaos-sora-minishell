/* ************************************************************************** */
/* main.c (Readline Fix & Safe Color)                                         */
/* ************************************************************************** */

#include "executor.h"
#include "garbage_collector.h"
#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>

/* * Readline Fix:
 * \001 ve \002 karakterleri, aradaki kodların (renklerin) görünmez olduğunu 
 * readline'a bildirir. Bu sayede imleç kayması ve satır taşma hataları düzelir.
 */
static char	*get_prompt(t_shell *shell)
{
	char	*prompt;
	char	*reset;
	char	*temp;

	reset = "\001\033[0m\002";
	
	if (shell->terminal_bg_color)
	{
		temp = gc_strjoin(shell->cmd_arena, "\001", shell->terminal_bg_color);
		temp = gc_strjoin(shell->cmd_arena, temp, "\002");
		prompt = temp;
	}
	else
		prompt = gc_strdup(shell->cmd_arena, "");

	if (shell->terminal_name_color)
	{
		temp = gc_strjoin(shell->cmd_arena, "\001", shell->terminal_name_color);
		temp = gc_strjoin(shell->cmd_arena, temp, "\002");
		prompt = gc_strjoin(shell->cmd_arena, prompt, temp);
	}

	if (shell->terminal_name)
		prompt = gc_strjoin(shell->cmd_arena, prompt, shell->terminal_name);
	else
		prompt = gc_strjoin(shell->cmd_arena, prompt, "minishell");

	prompt = gc_strjoin(shell->cmd_arena, prompt, reset);
	prompt = gc_strjoin(shell->cmd_arena, prompt, "$ ");
	
	return (prompt);
}

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
	shell->alias_table = gc_calloc(shell->global_arena, 1, sizeof(t_env_table));
	shell->alias_table->buckets = gc_calloc(shell->global_arena, ENV_TABLE_SIZE,
			sizeof(t_env_bucket *));
	
	shell->terminal_name = "minishell";
	shell->terminal_name_color = NULL;
	shell->terminal_bg_color = NULL;
	
	init_history(shell);
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
	if (shell->global_arena)
		gc_destroy(shell->global_arena);
	if (shell->cmd_arena)
		gc_destroy(shell->cmd_arena);
}

int	main(int argc, char const *argv[], char **envp)
{
	t_shell	shell;
	char	*input;
	t_token	*tokens;

	(void)argc;
	(void)argv;
	init_shell(&shell, (char **)envp);
	shell.path_dirs = parse_path(&shell);
	setup_signals();
	while (1)
	{
		input = readline(get_prompt(&shell));
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		tokens = lexer(input, &shell);
		free(input);
		if (tokens)
		{
			shell.ast_root = parser(tokens, &shell);
			if (shell.ast_root)
			{
				executor_run(&shell);
			}
		}
		clean_loop(&shell);
	}
	clean_loop(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
}
