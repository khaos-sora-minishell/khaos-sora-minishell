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

static int	handle_heredoc_entry(t_redir *redir, int cnt, t_shell *shell)
{
	int	fd;

	redir->heredoc_tmpfile = get_heredoc_filename(cnt, shell);
	fd = open(redir->heredoc_tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror("heredoc open"), -1);
	read_heredoc_loop(fd, redir->delimiter, redir->should_expand, shell);
	close(fd);
	if (get_signal() == SIGINT)
		return (-1);
	redir->type = TOKEN_REDIR_IN;
	redir->file = redir->heredoc_tmpfile;
	return (0);
}

int	process_cmd_heredoc(t_cmd *cmd, t_shell *shell)
{
	t_redir	*curr;
	int		counter;

	if (create_heredoc_dir() == -1)
		return (-1);
	curr = cmd->redirs;
	counter = 0;
	while (curr)
	{
		if (curr->type == TOKEN_HEREDOC)
		{
			if (handle_heredoc_entry(curr, counter, shell) == -1)
				return (-1);
		}
		curr = (counter++, curr->next);
	}
	return (0);
}

void	clean_heredoc(t_cmd *cmd)
{
	t_redir	*redirection;

	if (!cmd || !cmd->redirs)
		return ;
	redirection = cmd->redirs;
	while (redirection)
	{
		if (redirection->heredoc_tmpfile)
			unlink(redirection->heredoc_tmpfile);
		redirection = redirection->next;
	}
}

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
#ifdef BONUS
	else if (ast->type == NODE_AND || ast->type == NODE_OR)
	{
		if (process_ast_heredocs(ast->left, shell) == -1)
			return (-1);
		if (process_ast_heredocs(ast->right, shell) == -1)
			return (-1);
	}
	else if (ast->type == NODE_SUBSHELL && ast->subshell_node)
		return (process_ast_heredocs(ast->subshell_node, shell));
#endif
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
#ifdef BONUS
	else if (ast->type == NODE_AND || ast->type == NODE_OR)
	{
		clean_ast_heredocs(ast->left);
		clean_ast_heredocs(ast->right);
	}
	else if (ast->type == NODE_SUBSHELL && ast->subshell_node)
		clean_ast_heredocs(ast->subshell_node);
#endif
}
