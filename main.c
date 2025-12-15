/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/12/07 00:00:00 by akivam            #+#    #+#             */
/* Updated: 2025/12/15 21:15:00 by akivam           ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include "libft.h"
#include "garbage_collector.h"

/*
 * Parse PATH environment variable into array
 * Hash Table'dan (env_table) şifresi çözülmüş PATH'i çeker.
 */
char	**parse_path(t_shell *shell)
{
	char	*path_value;

	// env_get otomatik deşifre edip verir
	path_value = env_get(shell->env_table, "PATH", shell->global_arena);
	if (!path_value)
		return (NULL);
	return (gc_split((t_gc_context *)shell->global_arena, path_value, ':'));
}

/*
 * Initialize shell structure
 */
void	init_shell(t_shell *shell, char **envp)
{
	t_gc_context	*gc;

	gc = gc_create();
	if (!gc)
	{
		ft_putendl_fd("Error: GC initialization failed", 2);
		exit(1);
	}
	shell->global_arena = gc;
	// Komut bazlı arena henüz ayrılmadı, loop içinde yönetilebilir
	shell->cmd_arena = gc; 

	// --- TERMİNAL AYARLARI ---
	shell->terminal_name = gc_strdup(gc, "minishell");
	shell->terminal_text_color = gc_strdup(gc, "");
	shell->terminal_bg_color = gc_strdup(gc, "");
	shell->alias_list = NULL;

	// --- ORTAM DEĞİŞKENLERİ (HASH TABLE) ---
	// Senin istediğin fonksiyon ismiyle başlatma:
	shell->env_table = initilaze_env_table(envp, gc);
	if (!shell->env_table)
	{
		ft_putendl_fd("Error: Environment table init failed", 2);
		exit(1);
	}

	// Execve için array oluştur ve PATH'i parse et
	shell->env_array = env_table_to_array(shell->env_table, gc);
	shell->path_dirs = parse_path(shell);

	shell->exit_status = 0;
	shell->ast_root = NULL;
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
}

/*
 * Cleanup shell resources
 */
void	cleanup_shell(t_shell *shell)
{
	if (shell->global_arena)
		gc_destroy(shell->global_arena);
	// Readline geçmişini temizle (opsiyonel)
	rl_clear_history();
}

/*
 * Main shell loop
 */
static void	shell_loop(t_shell *shell)
{
	char		*line;
	char		*prompt;
	t_token		*tokens;
	t_gc_context	*gc;

	gc = (t_gc_context *)shell->global_arena;
	while (1)
	{
		// Prompt oluştur (Renkler ve isim dinamik olabilir)
		prompt = gc_strjoin(gc, shell->terminal_name, " > ");
		line = readline(prompt);

		if (!line) // Ctrl+D (EOF)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line); // Boş satırları kaydetme
		
		// 1. LEXER: Girdiyi tokenlara ayır
		tokens = lexer(line, shell);
		if (!tokens)
		{
			free(line); // Readline malloc'unu manuel free et (veya gc_track)
			continue ;
		}

		// 2. PARSER: Tokenları AST'ye çevir
		// Not: Parser hata durumunda NULL dönebilir ve hata mesajı basmalıdır.
		shell->ast_root = parser(tokens, shell);

		// 3. EXECUTOR: AST'yi çalıştır
		if (shell->ast_root)
		{
			// Executor.c dosyasındaki ana çalıştırma fonksiyonu
			// Bu fonksiyon AST üzerinde gezinerek komutları işletir
			executor_run(shell);
		}

		// Döngü sonu temizliği (Satır ve cmd_arena temizliği burada yapılabilir)
		free(line);
	}
}

/*
 * Main entry point
 */
int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;

	// 1. Başlatma (Hash Table & GC)
	init_shell(&shell, envp);

	// 2. Sinyal Kurulumu
	setup_signals();

	// 3. Ana Döngü (Oku-Ayrıştır-Çalıştır)
	shell_loop(&shell);

	// 4. Çıkış ve Temizlik
	cleanup_shell(&shell);

	return (shell.exit_status);
}
