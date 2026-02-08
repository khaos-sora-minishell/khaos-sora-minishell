/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:06:13 by harici            #+#    #+#             */
/*   Updated: 2026/02/07 18:05:13 by akivam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tetris.h"

static void	draw_border(char *buffer, int *pos, int w)
{
	int	i;

	append_string(buffer, pos, "  +");
	i = 0;
	while (i < w)
	{
		append_string(buffer, pos, "--");
		i++;
	}
	append_string(buffer, pos, "+\n");
}

static void	render_next_piece(t_tetris *t, char *buffer, int *pos, int y)
{
	int	px;
	int	py;

	append_string(buffer, pos, " ");
	py = y - 2;
	px = 0;
	while (px < 4)
	{
		if (t->pieces[t->next_piece][0][py][px])
			append_string(buffer, pos, "[]");
		else
			append_string(buffer, pos, "  ");
		px++;
	}
}

static void	render_right_panel(t_tetris *t, char *buffer, int *pos, int y)
{
	if (y == 0)
		append_string(buffer, pos, " Next:");
	else if (y >= 2 && y < 6)
		render_next_piece(t, buffer, pos, y);
	else if (y == 8)
		append_string(buffer, pos, " Score:");
	else if (y == 9)
	{
		append_string(buffer, pos, " ");
		append_int(buffer, pos, t->score);
	}
	else if (y == 11)
		append_string(buffer, pos, " Level:");
	else if (y == 12)
	{
		append_string(buffer, pos, " ");
		append_int(buffer, pos, t->level);
	}
}

void	render(t_tetris *t)
{
	char	buffer[TETRIS_BUFFER_SIZE];
	int		pos;
	int		y;

	if (t->game_over)
	{
		render_game_over(t);
		return ;
	}
	pos = 0;
	t->ghost_y = get_ghost_y(t);
	append_string(buffer, &pos, HOME);
	draw_border(buffer, &pos, t->board_w);
	y = 0;
	while (y < t->board_h)
	{
		append_string(buffer, &pos, "  |");
		render_board_line(t, buffer, &pos, y);
		append_string(buffer, &pos, "|");
		render_right_panel(t, buffer, &pos, y);
		append_string(buffer, &pos, "\033[K\n");
		y++;
	}
	draw_border(buffer, &pos, t->board_w);
	write(STDOUT_FILENO, buffer, pos);
}
