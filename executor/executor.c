/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:38 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 16:37:10 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	executor_run(t_shell *shell)
{
	if (!shell || !shell->ast_root)
		return ;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	if (process_ast_heredocs(shell->ast_root, shell) == 0)
		execute_ast(shell->ast_root, shell);
	clean_ast_heredocs(shell->ast_root);
	dup2(shell->stdin_backup, STDIN_FILENO);
	dup2(shell->stdout_backup, STDOUT_FILENO);
	close(shell->stdin_backup);
	close(shell->stdout_backup);
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
}
