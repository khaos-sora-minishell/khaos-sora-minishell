/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 21:42:26 by akivam            #+#    #+#             */
/*   Updated: 2026/01/01 21:47:21 by akivam           ###   ########.fr       */
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

static char	*read_heredoc_input(char *prompt, int interactive)
{
	char	*line;
	char	*tmp;

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
	return (line);
}

void	read_heredoc_loop(int fd, char *delim, t_shell *shell)
{
	char	*prompt;
	char	*line;
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
		line = read_heredoc_input(prompt, interactive);
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
