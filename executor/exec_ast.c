/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 21:39:22 by akivam            #+#    #+#             */
/*   Updated: 2025/12/07 20:44:27 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

/*
 * Execute subshell in forked process
 */
static void	execute_subshell(t_ast_node *ast, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		shell->exit_status = 1;
		return ;
	}
	if (pid == 0)
	{
		execute_ast(ast->subshell_node, shell);
		exit(shell->exit_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
}

/*
 ** Main executor girişi - Parser'dan gelen AST'yi execute eder
 ** B<u fonksiyon main. c'den çağrılır
 */
void	execute_ast(t_ast_node *ast, t_shell *shell)
{
	if (!ast || !shell)
		return ;
	if (ast->type == NODE_CMD)
		execute_command(ast->cmd, shell);
	else if (ast->type == NODE_PIPE)
		execute_pipe(ast->left, ast->right, shell);
	else if (ast->type == NODE_AND)
	{
		execute_ast(ast->left, shell);
		if (shell->exit_status == 0)
			execute_ast(ast->right, shell);
	}
	else if (ast->type == NODE_OR)
	{
		execute_ast(ast->left, shell);
		if (shell->exit_status != 0)
			execute_ast(ast->right, shell);
	}
	else if (ast->type == NODE_SUBSHELL)
		execute_subshell(ast, shell);
}
