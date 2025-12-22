/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 21:39:22 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 15:55:31 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

static void	exec_subshell(t_ast_node *node, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		shell->exit_status = 1;
		return ;
	}
	if (pid == 0)
	{
		setup_child_signals();
		execute_ast(node->left, shell);
		cleanup_shell(shell);
		exit(shell->exit_status);
	}
	ignore_signals();
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

void	execute_ast(t_ast_node *node, t_shell *shell)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
		execute_pipe(node->left, node->right, shell);
	else if (node->type == NODE_AND)
	{
		execute_ast(node->left, shell);
		if (shell->exit_status == 0)
			execute_ast(node->right, shell);
	}
	else if (node->type == NODE_OR)
	{
		execute_ast(node->left, shell);
		if (shell->exit_status != 0)
			execute_ast(node->right, shell);
	}
	else if (node->type == NODE_SUBSHELL)
		exec_subshell(node, shell);
	else
		execute_command(node->cmd, shell);
}
