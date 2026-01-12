/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 21:39:22 by akivam            #+#    #+#             */
/*   Updated: 2026/01/13 00:00:00 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

static void	exec_subshell(t_ast_node *ast, t_shell *shell)
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
		setup_child_signals();
		if (ast->subshell_node)
			execute_ast(ast->subshell_node, shell);
		cleanup_child_process(shell);
		exit(shell->exit_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
}

int	execute_ast(t_ast_node *ast, t_shell *shell)
{
	if (!ast || !shell)
		return (1);
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
		exec_subshell(ast, shell);
	return (shell->exit_status);
}
