/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 21:11:56 by akivam            #+#    #+#             */
/*   Updated: 2026/02/06 20:24:42 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "utils.h"

int	process_char(char c, char *buffer, int *pos, int *len)
{
	int	ret;

	if (c == 27)
		handle_escape_sequence(pos, len);
	else
	{
		ret = handle_special_char(c, buffer, pos, len);
		if (ret == 1)
			return (1);
		if (ret == -1)
			add_char_to_buffer(c, buffer, pos, len);
	}
	return (0);
}

static int	read_raw_input(char *buffer, int *len, char *last_char)
{
	int		pos;
	char	c;

	pos = 0;
	*len = 0;
	*last_char = 0;
	while (read(STDIN_FILENO, &c, 1) == 1)
	{
		*last_char = c;
		if (process_char(c, buffer, &pos, len))
			break ;
	}
	buffer[*len] = '\0';
	return (0);
}

char	*read_line_raw(char *prompt)
{
	char	buffer[4096];
	int		len;
	char	last_char;

	ft_putstr_fd(prompt, STDOUT_FILENO);
	read_raw_input(buffer, &len, &last_char);
	if (get_signal() != SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	if (get_signal() == SIGINT)
		return (NULL);
	if (last_char == 4 && len == 0)
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
