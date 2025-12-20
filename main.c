/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 16:59:22 by akivam            #+#    #+#             */
/*   Updated: 2025/12/20 20:51:48 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "garbage_collector.h"
#include "libft.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>

volatile sig_atomic_t	g_signal = 0;

/*
 * Prompt Oluşturucu
 * Renk ve Terminal ismine göre dinamik prompt döndürür.
 * Örn: (YEŞİL)minishell$ (RESET)
 */
/*
 * Prompt Oluşturucu (Tam Özelleştirilebilir)
 * Yapı: [ARKA_PLAN] + [YAZI_RENGİ] + [İSİM] + [RESET] + "$ "
 */
static char	*get_prompt(t_shell *shell)
{
	char	*prompt;
	char	*reset;

	reset = "\033[0m";
	// 1. Arkaplan Rengi (Easter egg ile değişebilir, varsayılan NULL)
	if (shell->terminal_bg_color)
		prompt = gc_strdup(shell->cmd_arena, shell->terminal_bg_color);
	else
		prompt = gc_strdup(shell->cmd_arena, "");
	// 2. İsim Rengi
	if (shell->terminal_name_color)
		prompt = gc_strjoin(shell->cmd_arena, prompt,
				shell->terminal_name_color);
	// 3. Terminal İsmi (Easter egg ile "Matrix" vs. olabilir)
	if (shell->terminal_name)
		prompt = gc_strjoin(shell->cmd_arena, prompt, shell->terminal_name);
	else
		prompt = gc_strjoin(shell->cmd_arena, prompt, "minishell");
	// 4. Reset ve "$ "
	prompt = gc_strjoin(shell->cmd_arena, prompt, reset);
	prompt = gc_strjoin(shell->cmd_arena, prompt, "$ ");
	return (prompt);
}

/* initilaze shell setup env and global arena*/

void	init_shell(t_shell *shell, char **envp)
{
	ft_bzero(shell, sizeof(t_shell));
	// Arenalar
	shell->global_arena = gc_create();
	shell->cmd_arena = gc_create();
	if (!shell->global_arena || !shell->cmd_arena)
		exit(1);
	// Env ve Array
	// ARTIK BURADA OTOMATİK OLARAK PS1 ve PS2 YÜKLENİYOR!
	shell->env_table = initilaze_env_table(envp, shell->global_arena);
	shell->env_array = env_table_to_array(shell->env_table,
			shell->global_arena);
	// Alias Tablosu
	shell->alias_table = gc_calloc(shell->global_arena, 1, sizeof(t_env_table));
	shell->alias_table->buckets = gc_calloc(shell->global_arena, ENV_TABLE_SIZE,
			sizeof(t_env_bucket *));
	// Görünüm (Struct içi varsayılanlar)
	shell->terminal_name = "minishell";
	shell->terminal_name_color = NULL;
	shell->terminal_bg_color = NULL;
	// Pencere adını değiştir (Easter Egg)
	set_terminal_name(shell->terminal_name);
	// History Başlat
	init_history(shell);
	shell->exit_status = 0;
}

/*her komut satırı sonraso yapılan temizlik*/
static void	clean_loop(t_shell *shell)
{
	gc_scope_pop_all(shell->cmd_arena);
	g_signal = 0;
	shell->ast_root = NULL;
}

/*program çıkışında yapılan tam temizlik*/
void	cleanup_shell(t_shell *shell)
{
	save_history_to_file(shell);
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
	setup_signals();
	while (1)
	{
		input = readline(get_prompt(&shell));
		if (!input)
		{
			printf("exit\n");
			break ;
			;
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
	return (shell.exit_status);
}
