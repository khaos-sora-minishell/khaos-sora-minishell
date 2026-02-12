/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 01:50:11 by akivam            #+#    #+#             */
/*   Updated: 2026/02/06 20:24:13 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "libft.h"
#include "shell.h"
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include "utils.h"

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

static char	*read_heredoc_input(char *prompt)
{
	struct termios	old_term;
	char			*line;

	if (!isatty(STDIN_FILENO))
	{
		ft_putstr_fd(prompt, STDOUT_FILENO);
		return (get_next_line(STDIN_FILENO));
	}
	setup_raw_mode(&old_term);
	line = read_line_raw(prompt);
	tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
	return (line);
}

void	read_heredoc_loop(int fd, t_redir *redir, t_shell *shell)
{
	char	*prompt;
	char	*line;

	prompt = env_get(shell->env_table, "PS2", shell->cmd_arena);
	if (!prompt)
		prompt = "> ";
	set_signal(0);
	while (1)
	{
		line = read_heredoc_input(prompt);
		if (!line)
		{
			if (get_signal() != SIGINT)
				ft_err_printf("minishell: warning: here-document at line %d "
					"delimited by end-of-file (wanted `%s')\n",
					shell->cmd_count, redir->delimiter);
			break ;
		}
		if (get_signal() == SIGINT)
			break ;
		if (process_heredoc_line(line, fd, redir, shell))
			break ;
	}
	setup_signals();
}

char	*get_heredoc_filename(int counter, t_shell *shell)
{
	static int	session_id = 0;
	char		*user;
	char		*base;

	if (session_id == 0)
		session_id = (int)((size_t)shell % 100000);
	user = getenv("USER");
	if (!user)
		user = "unknown";
	base = gc_strjoin(shell->cmd_arena, "/tmp/msh_hd_", user);
	base = gc_strjoin(shell->cmd_arena, base, "_");
	base = gc_strjoin(shell->cmd_arena, base,
			gc_itoa(shell->cmd_arena, session_id));
	base = gc_strjoin(shell->cmd_arena, base, "_");
	return (gc_strjoin(shell->cmd_arena, base,
			gc_itoa(shell->cmd_arena, counter)));
}
