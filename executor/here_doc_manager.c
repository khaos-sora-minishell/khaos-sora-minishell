/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 11:35:14 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 12:19:32 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "utils.h"

static void	read_heredoc_loop(int fd, char *delim, t_shell *shell)
{
	char	*line;
	char	*prompt;

	prompt = env_get(shell->env_table, "PS2", shell->cmd_arena);
	if (!prompt)
		prompt = "> ";
	while (1)
	{
		line = readline(prompt);
		if (!line)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

static char	*get_heredoc_filename(int counter, t_shell *shell)
{
	char	*tmp_file;

	tmp_file = gc_strjoin(shell->cmd_arena, "/tmp/minishell_heredocs/heredoc_",
			gc_itoa(shell->cmd_arena, counter));
	return (tmp_file);
}

static int	create_heredoc_dir(void)
{
	struct stat	_stat;
	char		*heredoc_dir;

	heredoc_dir = "/tmp/minishell_heredocs";
	if (stat(heredoc_dir, &_stat) == -1)
	{
		if (mkdir(heredoc_dir, 0777) == -1)
		{
			perror("minishell: heredoc dir");
			return (-1);
		}
	}
	return (0);
}

int	process_cmd_heredoc(t_cmd *cmd, t_shell *shell)
{
	t_redir	*current;
	int		counter;
	int		fd;

	if (create_heredoc_dir() == -1)
		return (-1);
	current = cmd->redirs;
	counter = 0;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			current->heredoc_tmpfile = get_heredoc_filename(counter, shell);
			fd = open(current->heredoc_tmpfile, O_WRONLY | O_CREAT | O_TRUNC,
					0644);
			if (fd == -1)
				return (perror("heredoc open"), -1);
			read_heredoc_loop(fd, current->delimiter, shell);
			close(fd);
			current->type = TOKEN_REDIR_IN;
			current->file = current->heredoc_tmpfile;
		}
		current = (counter++, current->next);
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
		{
			unlink(redirection->heredoc_tmpfile);
		}
		redirection = redirection->next;
	}
}
