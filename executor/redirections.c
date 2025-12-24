/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 19:20:38 by akivam            #+#    #+#             */
/*   Updated: 2025/12/07 20:40:17 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "utils.h"

/*
 * Input redirection (<)
 */
static int	handle_redir_in(t_redir *redir)
{
	int	fd;

	fd = open_file(redir->file);
	if (fd == -1)
	{
		perror(redir->file);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

/*
 * Output redirections (>, >>)
 */
static int	handle_redir_out(t_redir *redir)
{
	int	fd;

	if (redir->type == TOKEN_REDIR_OUT)
		fd = open_file_write(redir->file);
	else
		fd = open_file_append(redir->file);
	if (fd == -1)
	{
		perror(redir->file);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

/*
 * Heredoc redirection (<<)
 */
static int	handle_heredoc(t_redir *redir)
{
	int	fd;

	fd = open_file(redir->heredoc_tmpfile);
	if (fd == -1)
	{
		perror("heredoc");
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

/*
 * Yönlendirmeleri setup eder - <, >, >>, <<
 */
int	setup_redirections(t_redir *redirs, t_shell *shell)
{
	t_redir	*current_redir;

	(void)shell;
	current_redir = redirs;
	while (current_redir)
	{
		if (current_redir->type == TOKEN_REDIR_IN)
		{
			if (handle_redir_in(current_redir) == -1)
				return (-1);
		}
		else if (current_redir->type == TOKEN_REDIR_OUT
			|| current_redir->type == TOKEN_REDIR_APPEND)
		{
			if (handle_redir_out(current_redir) == -1)
				return (-1);
		}
		else if (current_redir->type == TOKEN_HEREDOC)
		{
			if (handle_heredoc(current_redir) == -1)
				return (-1);
		}
		current_redir = current_redir->next;
	}
	return (0);
}
