/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:36:28 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 19:36:28 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands.h"
#include <termios.h>

void	setup_tetris_terminal(struct termios *old)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, old);
	raw = *old;
	raw.c_lflag &= ~(ICANON | ECHO | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
	write(1, CLEAR, 4);
	write(1, HOME, 3);
}
