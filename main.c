/* ************************************************************************** */
/*                                                                            */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by akivam            #+#    #+#             */
/*   Updated: 2025/12/07 00:00:00 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include "libft.h"

/*
 * Parse PATH environment variable into array
 * (Header'da tanımlı olduğu için static değil)
 */
char	**parse_path(t_env *env, void *arena)
{
	char			*path_value;
	char			**dirs;

	(void)arena;  // Şimdilik kullanılmıyor
	
	// Get PATH value from env
	path_value = get_env_value(env, "PATH");
	if (!path_value)
		return (NULL);
	
	// Split by ':'
	dirs = ft_split(path_value, ':');  // ⚠️ ft_split uses malloc, needs manual free
	
	// TODO: Bu dirs'i GC ile yönetmek için gc_strdup ile kopyala
	// Şimdilik ft_split'in malloc'unu kullanıyoruz
	
	return (dirs);
}

/*
 * Initialize shell structure
 * (Header'da tanımlı olduğu için static değil ve void return)
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
	shell->cmd_arena = gc;
	
	// Initialize environment
	shell->env_list = init_env(envp, gc);
	if (!shell->env_list)
	{
		ft_putendl_fd("Error: Environment initialization failed", 2);
		exit(1);
	}
	
	shell->env_array = env_list_to_array(shell->env_list, gc);
	
	// ✅ FIX: Parse PATH with correct signature
	shell->path_dirs = parse_path(shell->env_list, gc);
	
	shell->exit_status = 0;
	shell->ast_root = NULL;
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
}

/*
 * Cleanup shell resources
 * (Header'da tanımlı olduğu için static değil)
 */
void	cleanup_shell(t_shell *shell)
{
	int	i;

	// ⚠️ path_dirs ft_split ile allocate edildi, manuel free gerekli
	if (shell->path_dirs)
	{
		i = 0;
		while (shell->path_dirs[i])
		{
			free(shell->path_dirs[i]);
			i++;
		}
		free(shell->path_dirs);
	}
	
	if (shell->global_arena)
		gc_destroy(shell->global_arena);
}

/*
 * TEMPORARY: Create mock AST for testing
 */
static t_ast_node	*create_mock_ast(t_shell *shell, char *input)
{
	t_ast_node		*node;
	t_cmd			*cmd;
	t_gc_context	*gc;

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
	cmd->args = ft_split(input, ' ');  // ⚠️ Manual malloc
	
	node->cmd = cmd;
	return (node);
}

/*
 * Clean mock AST args
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
		
		if (!line)  // Ctrl+D
		{
			printf("exit\n");
			break ;
		}
		
		if (*line)
			add_history(line);
		
		if (!*line)  // Empty line
		{
			free(line);
			continue ;
		}
		
		// TEMPORARY: Create mock AST
		shell->ast_root = create_mock_ast(shell, line);
		
		if (shell->ast_root)
		{
			executor_run(shell);
			
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

	// init_shell artık void, exit ile çıkıyor hata durumunda
	init_shell(&shell, envp);

	shell_loop(&shell);

	cleanup_shell(&shell);

	return (shell.exit_status);
}