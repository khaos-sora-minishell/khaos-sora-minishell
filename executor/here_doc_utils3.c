/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 21:11:56 by akivam            #+#    #+#             */
/*   Updated: 2026/01/04 21:14:25 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "utils.h"

static char	*read_raw_input(char *buffer, int *len)
{
	int		pos;
	char	c;

	pos = 0;
	*len = 0;
	while (read(STDIN_FILENO, &c, 1) == 1)
	{
		if (process_char(c, buffer, &pos, len))
			break ;
	}
	buffer[*len] = '\0';
	return (buffer);
}

char	*read_line_raw(char *prompt)
{
	char	buffer[4096];
	int		len;

	ft_putstr_fd(prompt, STDOUT_FILENO);
	read_raw_input(buffer, &len);
	if (get_signal() != SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	if (get_signal() == SIGINT)
		return (NULL);
	return (ft_strdup(buffer));
}

static int	handle_heredoc_entry(t_redir *redir, int cnt, t_shell *shell)
{
	int	fd;

	redir->heredoc_tmpfile = get_heredoc_filename(cnt, shell);
	fd = open(redir->heredoc_tmpfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror("heredoc open"), -1);
	read_heredoc_loop(fd, redir, shell);
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
