/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:38 by akivam            #+#    #+#             */
/*   Updated: 2025/11/19 19:20:38 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

// ! bu fonksiyon mainde direkt çağrılacak asty execute ediyor
void	executor_run(t_shell *shell)
{
	if (!shell || !shell->ast_root)
		return ;
	// redirection için fdler yedeklendi
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	// ast execute
	execute_ast(shell->ast_root, shell);
	// restore fd
	dup2(shell->stdin_backup, STDIN_FILENO);
	dup2(shell->stdout_backup, STDOUT_FILENO);
	close(shell->stdin_backup);
	close(shell->stdout_backup);
}
