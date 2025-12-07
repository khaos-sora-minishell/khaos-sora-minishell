/* ************************************************************************** */
/*                                                                            */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by akivam            #+#    #+#             */
/*   Updated: 2025/12/06 00:00:00 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>

/*
 * Initialize shell structure
 */
static int	init_shell(t_shell *shell, char **envp)
{
	t_gc_context	*gc;

	gc = gc_create();
	if (!gc)
		return (0);
	shell->global_arena = gc;
	shell->cmd_arena = gc; // Şimdilik aynı arena
	shell->env_list = init_env(envp, gc);
	shell->env_array = env_list_to_array(shell->env_list, gc);
	shell->exit_status = 0;
	shell->ast_root = NULL;
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	return (1);
}

/*
 * Cleanup shell resources
 */
static void	cleanup_shell(t_shell *shell)
{
	if (shell->global_arena)
		gc_destroy(shell->global_arena);
}

/*
 * TEMPORARY: Create mock AST for testing
 * Bu fonksiyon parser tamamlanınca silinecek
 */
static t_ast_node	*create_mock_ast(t_shell *shell, char *input)
{
	t_ast_node		*node;
	t_cmd			*cmd;
	t_gc_context	*gc;

	gc = gc_get_current();
	// Basit bir komut node'u oluştur
	node = gc_malloc(gc, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->left = NULL;
	node->right = NULL;
	node->subshell_node = NULL;
	// Komut yapısını oluştur
	cmd = gc_malloc(gc, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->redirs = NULL;
	// Basit parsing: boşluklara göre ayır
	cmd->args = ft_split(input, ' '); // Manuel malloc!
	node->cmd = cmd;
	return (node);
}

/*
 * Clean mock AST args (ft_split manual malloc kullanır)
 */
static void	cleanup_mock_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

/*
 * Main shell loop
 */
static void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("minishell> ");
		if (!line) // Ctrl+D
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		// Boş satırı atla
		if (!*line)
		{
			free(line);
			continue ;
		}
		// TEMPORARY: Mock AST oluştur
		shell->ast_root = create_mock_ast(shell, line);
		if (shell->ast_root)
		{
			// Execute AST
			executor_run(shell);
			// Cleanup
			if (shell->ast_root->cmd && shell->ast_root->cmd->args)
				cleanup_mock_args(shell->ast_root->cmd->args);
		}
		free(line);
	}
}

/*
 * Main entry point
 */
int	main(int ac, char **av, char **envp)
{
	t_shell shell;

	(void)ac;
	(void)av;

	if (!init_shell(&shell, envp))
	{
		fprintf(stderr, "minishell: initialization failed\n");
		return (1);
	}

	// Main loop
	shell_loop(&shell);

	// Cleanup
	cleanup_shell(&shell);

	return (shell.exit_status);
}