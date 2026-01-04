/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 11:35:14 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 13:37:45 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef BONUS

int	process_ast_heredocs(t_ast_node *ast, t_shell *shell)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD && ast->cmd)
		return (process_cmd_heredoc(ast->cmd, shell));
	else if (ast->type == NODE_PIPE)
	{
		if (process_ast_heredocs(ast->left, shell) == -1)
			return (-1);
		if (process_ast_heredocs(ast->right, shell) == -1)
			return (-1);
	}
	else if (ast->type == NODE_AND || ast->type == NODE_OR)
	{
		if (process_ast_heredocs(ast->left, shell) == -1)
			return (-1);
		if (process_ast_heredocs(ast->right, shell) == -1)
			return (-1);
	}
	else if (ast->type == NODE_SUBSHELL && ast->subshell_node)
		return (process_ast_heredocs(ast->subshell_node, shell));
	return (0);
}

void	clean_ast_heredocs(t_ast_node *ast)
{
	if (!ast)
		return ;
	if (ast->type == NODE_CMD && ast->cmd)
		clean_heredoc(ast->cmd);
	else if (ast->type == NODE_PIPE)
	{
		clean_ast_heredocs(ast->left);
		clean_ast_heredocs(ast->right);
	}
	else if (ast->type == NODE_AND || ast->type == NODE_OR)
	{
		clean_ast_heredocs(ast->left);
		clean_ast_heredocs(ast->right);
	}
	else if (ast->type == NODE_SUBSHELL && ast->subshell_node)
		clean_ast_heredocs(ast->subshell_node);
}

#else

int	process_ast_heredocs(t_ast_node *ast, t_shell *shell)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD && ast->cmd)
		return (process_cmd_heredoc(ast->cmd, shell));
	else if (ast->type == NODE_PIPE)
	{
		if (process_ast_heredocs(ast->left, shell) == -1)
			return (-1);
		if (process_ast_heredocs(ast->right, shell) == -1)
			return (-1);
	}
	return (0);
}

void	clean_ast_heredocs(t_ast_node *ast)
{
	if (!ast)
		return ;
	if (ast->type == NODE_CMD && ast->cmd)
		clean_heredoc(ast->cmd);
	else if (ast->type == NODE_PIPE)
	{
		clean_ast_heredocs(ast->left);
		clean_ast_heredocs(ast->right);
	}
}

#endif
