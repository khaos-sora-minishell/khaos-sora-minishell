/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/12/07 00:00:00 by akivam            #+#    #+#             */
/* Updated: 2025/12/10 23:00:00 by akivam           ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include "libft.h"
#include "garbage_collector.h"

/* Alias fonksiyonunun prototipi (Header'da yoksa diye buraya ekledim) */
/* Eğer minishell.h içine eklediysen burayı silebilirsin */
char *replace_alias(char *line, t_shell *shell); 

/*
 * Parse PATH environment variable into array
 */
char **parse_path(t_shell *shell) // Argüman değişti
{
    char *path_value;
    
    // env_get otomatik deşifre edip verir
    path_value = env_get(shell->env_table, "PATH", shell->global_arena);
    if (!path_value)
        return (NULL);
    return (gc_split((t_gc_context *)shell->global_arena, path_value, ':'));
}

/*
 * Initialize shell structure
 */
void    init_shell(t_shell *shell, char **envp)
{
    t_gc_context    *gc;

    gc = gc_create();
    if (!gc)
    {
        ft_putendl_fd("Error: GC initialization failed", 2);
        exit(1);
    }
    
    shell->global_arena = gc;
    shell->cmd_arena = gc; 
    
    // --- BAŞLANGIÇ AYARLARI ---
    shell->terminal_name = gc_strdup(gc, "minishell"); // Varsayılan isim
    shell->terminal_text_color = gc_strdup(gc, "");
    shell->terminal_bg_color = gc_strdup(gc, "");
    
    shell->alias_list = NULL; // Alias listesi boş başlar
    // --------------------------

    // Initialize environment
    shell->env_list = initilaze_env_table(envp, gc);
    if (!shell->env_list)
    {
        ft_putendl_fd("Error: Environment initialization failed", 2);
        exit(1);
    }
    
    shell->env_array = env_list_to_array(shell->env_list, gc);
    shell->path_dirs = parse_path(shell->env_list, gc);
    
    shell->exit_status = 0;
    shell->ast_root = NULL;
    shell->stdin_backup = -1;
    shell->stdout_backup = -1;
}

/*
 * Cleanup shell resources
 */
void    cleanup_shell(t_shell *shell)
{
    if (shell->global_arena)
        gc_destroy(shell->global_arena);
}

/*
 * TEMPORARY: Create mock AST for testing
 */
static t_ast_node   *create_mock_ast(t_shell *shell, char *input)
{
    t_ast_node      *node;
    t_cmd           *cmd;
    t_gc_context    *gc;

    gc = (t_gc_context *)shell->global_arena;
    
    node = gc_malloc(gc, sizeof(t_ast_node));
    if (!node)
        return (NULL);
    
    node->type = NODE_CMD;
    node->left = NULL;
    node->right = NULL;
    node->subshell_node = NULL;
    
    cmd = gc_malloc(gc, sizeof(t_cmd));
    if (!cmd)
        return (NULL);
    
    cmd->redirs = NULL;
    cmd->args = gc_split(gc, input, ' ');
    
    node->cmd = cmd;
    return (node);
}

static void cleanup_mock_args(char **args)
{
    (void)args;
}

/*
 * Main shell loop
 */
static void shell_loop(t_shell *shell)
{
    char            *line;
    char            *prompt; 
    t_gc_context    *gc;

    gc = (t_gc_context *)shell->global_arena;
    while (1)
    {
        // --- DİNAMİK PROMPT OLUŞTURMA ---
        // shell->terminal_name her değiştiğinde prompt da değişir.
        prompt = gc_strjoin(gc, shell->terminal_name, " > ");
        
        line = readline(prompt);
        // --------------------------------

        if (!line)
        {
            printf("exit\n"); // Ctrl+D çıkışı
            break ;
        }
        
        gc_track(gc, line); // Readline malloc ettiği için GC'ye ekle
        
        if (*line)
            add_history(line);
        if (!*line)
            continue ;
        
        // --- ALIAS GENİŞLETME (Expansion) ---
        // Kullanıcı "ll" yazdıysa bunu "ls -l" yapar.
        // Eğer alias dosyanı eklediysen aşağıdaki satırı aktif et:
        // line = replace_alias(line, shell); 
        // ------------------------------------

        shell->ast_root = create_mock_ast(shell, line);
        if (shell->ast_root)
        {
            executor_run(shell);
            if (shell->ast_root->cmd && shell->ast_root->cmd->args)
                cleanup_mock_args(shell->ast_root->cmd->args);
        }
    }
}

/*
 * Main entry point
 */
int main(int ac, char **av, char **envp)
{
    t_shell shell;

    (void)ac;
    (void)av;

    init_shell(&shell, envp);
    setup_signals();

    shell_loop(&shell);

    cleanup_shell(&shell);

    return (shell.exit_status);
}