/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akivam <akivam@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 19:36:28 by akivam            #+#    #+#             */
/*   Updated: 2026/01/18 19:36:28 by akivam           ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "extra_commands_bonus.h"

static int	check_active_piece(t_tetris *t, int x, int y)
{
	int	px;
	int	py;

	py = y - t->pos_y;
	px = x - t->pos_x;
	if (px >= 0 && px < 4 && py >= 0 && py < 4)
	{
		if (t->pieces[t->current_piece][t->rotation][py][px])
			return (1);
	}
	return (0);
}

static void	draw_cell(t_tetris *t, int x, int y)
{
	int	draw;

	draw = t->board[y][x];
	if (!draw)
		draw = check_active_piece(t, x, y);
	if (draw)
		write(1, "[]", 2);
	else
		write(1, "  ", 2);
}

void	render(t_tetris *t)
{
	int	x;
	int	y;

	write(1, HOME, 3);
	write(1, CLEAR, 4);
	y = 0;
	while (y < t->board_h)
	{
		x = 0;
		while (x < t->board_w)
		{
			draw_cell(t, x, y);
			x++;
		}
		write(1, "\n", 1);
		y++;
	}
	ft_printf("\nScore: %d\n", t->score);
	ft_printf("Level: %d\n", t->level);
}
