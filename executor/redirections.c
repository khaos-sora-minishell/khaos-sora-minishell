/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:38 by akivam            #+#    #+#             */
/*   Updated: 2025/11/19 19:20:39 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Yönlendirmeleri setup eder - <, >, >>, <<
 */
int	setup_redirections(t_redir *redirs, t_shell *shell)
{
	t_redir	*current_redir;
	int		fd;

	(void)shell;
	current_redir = redirs;
	while (current_redir)
	{
		if (current_redir->type == TOKEN_REDIR_IN) // <
		{
			fd = open(current_redir->file, O_RDONLY);
			if (fd == -1)
			{
				perror(current_redir->file);
				return (-1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (current_redir->type == TOKEN_REDIR_OUT) // >
		{
			fd = open(current_redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(current_redir->file);
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (current_redir->type == TOKEN_REDIR_APPEND) // >>
		{
			fd = open(current_redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(current_redir->file);
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		
		else if (current_redir->type == TOKEN_HEREDOC) // <<
		{
			fd = open(current_redir->heredoc_tmpfile, O_RDONLY);
			if (fd == -1)
			{
				perror("heredoc");
				return (-1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		current_redir = current_redir->next;
	}
	return (0);
}
