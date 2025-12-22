/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 13:24:21 by akivam            #+#    #+#             */
/*   Updated: 2025/12/22 13:44:01 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "utils.h"
#include "get_next_line.h"


static int	heredoc_signal_checker(void)
{
	if (get_signal() == SIGINT)
		rl_done = 1;
	return (0);
}

static int	process_heredoc_line(char *line, char *delim, int fd)
{
	if (get_signal() == SIGINT)
	{
		if (line)
			free(line);
		return (1);
	}
	if (!line)
		return (1);
	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (1);
	}
	ft_putendl_fd(line, fd);
	free(line);
	return (0);
}

void	read_heredoc_loop(int fd, char *delim, t_shell *shell)
{
	char	*prompt;
	char	*line;
	char	*tmp;
	int		interactive;

	prompt = env_get(shell->env_table, "PS2", shell->cmd_arena);
	if (!prompt)
		prompt = "> ";
	interactive = isatty(STDIN_FILENO);
	set_signal(0);
	if (interactive)
		rl_event_hook = heredoc_signal_checker;
	while (1)
	{
		if (interactive)
			line = readline(prompt);
		else
		{
			tmp = get_next_line(STDIN_FILENO);
			if (!tmp)
				line = NULL;
			else
			{
				line = ft_strtrim(tmp, "\n");
				free(tmp);
			}
		}
		if (process_heredoc_line(line, delim, fd))
			break ;
	}
	if (interactive)
		rl_event_hook = NULL;
}


char	*get_heredoc_filename(int counter, t_shell *shell)
{
	return (gc_strjoin(shell->cmd_arena, "/tmp/minishell_heredocs/heredoc_",
			gc_itoa(shell->cmd_arena, counter)));
}

int	create_heredoc_dir(void)
{
	struct stat	st;

	if (stat("/tmp/minishell_heredocs", &st) == -1)
	{
		if (mkdir("/tmp/minishell_heredocs", 0777) == -1)
		{
			perror("minishell: heredoc dir");
			return (-1);
		}
	}
	return (0);
}
