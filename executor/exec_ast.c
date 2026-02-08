/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 21:39:22 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 16:45:41 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

int	execute_ast(t_ast_node *ast, t_shell *shell)
{
	if (!ast || !shell)
		return (1);
	if (ast->type == NODE_CMD)
		execute_command(ast->cmd, shell);
	else if (ast->type == NODE_PIPE)
		execute_pipe(ast->left, ast->right, shell);
	return (shell->exit_status);
}
