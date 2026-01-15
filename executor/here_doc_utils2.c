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
#include <stdlib.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include "utils.h"

static void	setup_raw_mode(struct termios *old_term)
{
	struct termios	new_term;

	tcgetattr(STDIN_FILENO, old_term);
	new_term = *old_term;
	new_term.c_lflag &= ~(ICANON | ECHO | ISIG);
	new_term.c_cc[VMIN] = 1;
	new_term.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

static int	handle_special_char(char c, char *buffer, int *pos, int *len)
{
	if (c == 3)
	{
		write(STDOUT_FILENO, "^C\n", 3);
		set_signal(SIGINT);
		return (1);
	}
	if (c == 127 || c == 8)
	{
		if (*pos > 0)
		{
			(*pos)--;
			(*len)--;
			ft_memmove(buffer + *pos, buffer + *pos + 1, *len - *pos);
			buffer[*len] = '\0';
			write(STDOUT_FILENO, "\b \b", 3);
		}
		return (0);
	}
	if (c == '\n')
		return (1);
	return (-1);
}

static void	handle_escape_sequence(int *pos, int *len)
{
	char	seq[3];

	if (read(STDIN_FILENO, &seq[0], 1) != 1)
		return ;
	if (seq[0] != '[')
		return ;
	if (read(STDIN_FILENO, &seq[1], 1) != 1)
		return ;
	if (seq[1] == 'C' && *pos < *len)
	{
		write(STDOUT_FILENO, "\x1b[C", 3);
		(*pos)++;
	}
	else if (seq[1] == 'D' && *pos > 0)
	{
		write(STDOUT_FILENO, "\x1b[D", 3);
		(*pos)--;
	}
}

static char	*read_line_raw(char *prompt)
{
	char			buffer[4096];
	int				pos;
	int				len;
	char			c;
	int				ret;

	ft_putstr_fd(prompt, STDOUT_FILENO);
	pos = 0;
	len = 0;
	while (1)
	{
		if (read(STDIN_FILENO, &c, 1) != 1)
			return (NULL);
		if (c == 27)
			handle_escape_sequence(&pos, &len);
		else
		{
			ret = handle_special_char(c, buffer, &pos, &len);
			if (ret == 1)
				break ;
			if (ret == 0)
				continue ;
			if (len < 4095)
			{
				ft_memmove(buffer + pos + 1, buffer + pos, len - pos);
				buffer[pos++] = c;
				len++;
				write(STDOUT_FILENO, &c, 1);
			}
		}
	}
	buffer[len] = '\0';
	if (get_signal() != SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	if (get_signal() == SIGINT)
		return (NULL);
	return (ft_strdup(buffer));
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
		if (!line || get_signal() == SIGINT)
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
