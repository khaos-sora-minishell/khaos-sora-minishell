/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 21:42:26 by akivam            #+#    #+#             */
/*   Updated: 2026/02/06 22:35:31 by akivam           ###   ########.fr       */
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

void	setup_raw_mode(struct termios *old_term)
{
	struct termios	new_term;

	tcgetattr(STDIN_FILENO, old_term);
	new_term = *old_term;
	new_term.c_lflag &= ~(ICANON | ECHO | ISIG);
	new_term.c_cc[VMIN] = 1;
	new_term.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

static int	handle_backspace(char *buffer, int *pos, int *len)
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

int	handle_special_char(char c, char *buffer, int *pos, int *len)
{
	if (c == 3)
	{
		write(STDOUT_FILENO, "^C\n", 3);
		set_signal(SIGINT);
		return (1);
	}
	if (c == 4 && *pos == 0 && *len == 0)
		return (1);
	if (c == 127 || c == 8)
		return (handle_backspace(buffer, pos, len));
	if (c == '\n')
		return (1);
	return (-1);
}

void	handle_escape_sequence(int *pos, int *len)
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

void	add_char_to_buffer(char c, char *buffer, int *pos, int *len)
{
	if (*len < 4095)
	{
		ft_memmove(buffer + *pos + 1, buffer + *pos, *len - *pos);
		buffer[(*pos)++] = c;
		(*len)++;
		write(STDOUT_FILENO, &c, 1);
	}
}
